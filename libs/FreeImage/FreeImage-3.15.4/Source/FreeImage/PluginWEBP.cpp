// ==========================================================
// WEBP Loader
//
// Design and implementation by
// - Dmitry Moskalchuk (dm@crystax.net)
//
// This file is part of FreeImage 3
//
// COVERED CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY
// OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES
// THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE
// OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED
// CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT
// THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY
// SERVICING, REPAIR OR CORRECTION. THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL
// PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER
// THIS DISCLAIMER.
//
// Use at your own risk!
// ==========================================================

#include "FreeImage.h"
#include "Utilities.h"

#include <webp/decode.h>

// ==========================================================
// Plugin Interface
// ==========================================================

static int s_format_id;

// ==========================================================
// Plugin Implementation
// ==========================================================

static const char * DLL_CALLCONV
Format() {
	return "WEBP";
}

static const char * DLL_CALLCONV
Description() {
	return "Google WebP image";
}

static const char * DLL_CALLCONV
Extension() {
	return "webp";
}

static const char * DLL_CALLCONV
RegExpr() {
	return "^RIFF....WEBP";
}

static const char * DLL_CALLCONV
MimeType() {
	return "image/webp";
}

static BOOL DLL_CALLCONV
Validate(FreeImageIO *io, fi_handle handle) {
	BYTE riff_signature[] = {'R', 'I', 'F', 'F'};
	BYTE webp_signature[] = {'W', 'E', 'B', 'P'};
	BYTE signature[12];

	if (io->read_proc(signature, 1, sizeof(signature), handle) < sizeof(signature))
		return FALSE;
	
	if (memcmp(riff_signature, signature, sizeof(riff_signature)) == 0 &&
		memcmp(webp_signature, signature + 8, sizeof(webp_signature)) == 0)
		return TRUE;

	return FALSE;
}

static BOOL DLL_CALLCONV
SupportsExportDepth(int depth) {
	return (depth == 32);
}

static BOOL DLL_CALLCONV
SupportsExportType(FREE_IMAGE_TYPE type) {
	return (type == FIT_BITMAP) ? TRUE : FALSE;
}

static BOOL DLL_CALLCONV
SupportsNoPixels() {
	return TRUE;
}

static FIBITMAP * DLL_CALLCONV
Load(FreeImageIO *io, fi_handle handle, int page, int flags, void *data) {
	if (!handle)
		return NULL;

	if (io->seek_proc(handle, 0, SEEK_END) == -1) {
		FreeImage_OutputMessageProc(s_format_id, "Can't seek to the end of the file");
		return NULL;
	}
	long size = io->tell_proc(handle);
	if (size == -1) {
		FreeImage_OutputMessageProc(s_format_id, "Can't tell current position in the file");
		return NULL;
	}
	if (io->seek_proc(handle, 0, SEEK_SET) == -1) {
		FreeImage_OutputMessageProc(s_format_id, "Can't seek to the begin of the file");
		return NULL;
	}

	uint8_t *imgdata = (uint8_t *)malloc(size);
	if (!imgdata) {
		FreeImage_OutputMessageProc(s_format_id, FI_MSG_ERROR_MEMORY);
		return NULL;
	}

	if (io->read_proc(imgdata, 1, size, handle) < size) {
		free(imgdata);
		FreeImage_OutputMessageProc(s_format_id, "Can't read file");
		return NULL;
	}

	int width, height;
	if (!WebPGetInfo(imgdata, size, &width, &height))
	{
		free(imgdata);
		FreeImage_OutputMessageProc(s_format_id, "Can't detect webp stream");
		return NULL;
	}

	BOOL header_only = (flags & FIF_LOAD_NOPIXELS) == FIF_LOAD_NOPIXELS;
	FIBITMAP *dib = FreeImage_AllocateHeader(header_only, width, height, 32,
		FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);
	if (!dib) {
		free(imgdata);
		FreeImage_OutputMessageProc(s_format_id, FI_MSG_ERROR_MEMORY);
		return NULL;
	}

	if (!header_only) {
		uint8_t *rgba = WebPDecodeRGBA(imgdata, size, &width, &height);
		if (!rgba) {
			free(imgdata);
			FreeImage_OutputMessageProc(s_format_id, "Can't decode image data");
			return NULL;
		}

		uint8_t *src = rgba;
		for (int y = height - 1; y >= 0; --y) {
			BYTE *bits = FreeImage_GetScanLine(dib, y);
			for (int x = 0; x < width; ++x) {
				bits[FI_RGBA_RED]   = src[0];
				bits[FI_RGBA_GREEN] = src[1];
				bits[FI_RGBA_BLUE]  = src[2];
				bits[FI_RGBA_ALPHA] = src[3];
				bits += 4;
				src += 4;
			}
		}
		FreeImage_SetTransparent(dib, TRUE);
		free(rgba);
	}

	free(imgdata);
	return dib;
}

// ==========================================================
//   Init
// ==========================================================

void DLL_CALLCONV
InitWEBP(Plugin *plugin, int format_id) {
	s_format_id = format_id;

	plugin->format_proc = Format;
	plugin->description_proc = Description;
	plugin->extension_proc = Extension;
	plugin->regexpr_proc = RegExpr;
	plugin->open_proc = NULL;
	plugin->close_proc = NULL;
	plugin->pagecount_proc = NULL;
	plugin->pagecapability_proc = NULL;
	plugin->load_proc = Load;
	plugin->save_proc = NULL; // not implemented yet
	plugin->validate_proc = Validate;
	plugin->mime_proc = MimeType;
	plugin->supports_export_bpp_proc = SupportsExportDepth;
	plugin->supports_export_type_proc = SupportsExportType;
	plugin->supports_icc_profiles_proc = NULL;	// not implemented yet;
	plugin->supports_no_pixels_proc = SupportsNoPixels;
}

/********************************************************************
 * PluginPVR.cpp
 * Created by Vladimir Lebedev-Schmidthof <dair@joybits.org>
 * at Mon Feb 16 22:11:25 MSK 2015
 *
 * (c) JoyBits, All Rights Reserved
 *******************************************************************/

#include "FreeImage.h"
#include "Utilities.h"

// ==========================================================
// Plugin Interface
// ==========================================================

static int s_format_id;

static const char* gPVRTexIdentifier = "PVR!";

// ==========================================================
// Plugin Implementation
// ==========================================================

static const char * DLL_CALLCONV
Format() {
	return "PVR";
}

static const char * DLL_CALLCONV
Description() {
	return "PVR: Compressed OpenGL texture";
}

static const char * DLL_CALLCONV
Extension() {
	return "pvr";
}

static const char * DLL_CALLCONV
RegExpr() {
	return "^................................................PVR!";
}

static const char * DLL_CALLCONV
MimeType() {
	return "image/pvr";
}

enum
{
	kPVRTextureFlagTypePVRTC_2 = 24,
	kPVRTextureFlagTypePVRTC_4
};

typedef struct _PVRTexHeader
{
	uint32_t headerLength;
	uint32_t height;
	uint32_t width;
	uint32_t numMipmaps;
	uint32_t flags;
	uint32_t dataLength;
	uint32_t bpp;
	uint32_t bitmaskRed;
	uint32_t bitmaskGreen;
	uint32_t bitmaskBlue;
	uint32_t bitmaskAlpha;
	uint32_t pvrTag;
	uint32_t numSurfs;
} PVRTexHeader;

#define PVR_TEXTURE_FLAG_TYPE_MASK 0xff

static BOOL DLL_CALLCONV
Validate(FreeImageIO *io, fi_handle handle) {
    struct _PVRTexHeader header;
	if (io->read_proc(&header, 1, sizeof(header), handle) < sizeof(header))
		return FALSE;
	
	if (memcmp(&header.pvrTag, gPVRTexIdentifier, sizeof(header.pvrTag)) == 0)
        return TRUE;
	return FALSE;
}

static FIBITMAP * DLL_CALLCONV
Load(FreeImageIO *io, fi_handle handle, int page, int flags, void *data) {
    if (!handle)
        return NULL;
    
    struct _PVRTexHeader header;
    
    if (io->read_proc(&header, 1, sizeof(header), handle) < sizeof(header))
    {
        FreeImage_OutputMessageProc(s_format_id, "Can't read header from file");
        return NULL;
    }
    
    BOOL header_only = (flags & FIF_LOAD_NOPIXELS) == FIF_LOAD_NOPIXELS;
    // allocating memory
    
    uint32_t dataSize = 0;
    
    if (!header_only)
    {
        uint32_t dataOffset = 0;
        uint32_t formatFlags = header.flags & PVR_TEXTURE_FLAG_TYPE_MASK;
        uint32_t blockSize = 0, widthBlocks = 0, heightBlocks = 0, bpp = 0;
        
        if (dataOffset < header.dataLength)
        {
            if (formatFlags == kPVRTextureFlagTypePVRTC_4)
            {
                blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
                widthBlocks = header.width / 4;
                heightBlocks = header.height / 4;
                bpp = 4;
            }
            else
            {
                blockSize = 8 * 4; // Pixel by pixel block size for 2bpp
                widthBlocks = header.width / 8;
                heightBlocks = header.height / 4;
                bpp = 2;
            }
            
            // Clamp to minimum number of blocks
            if (widthBlocks < 2)
                widthBlocks = 2;
            if (heightBlocks < 2)
                heightBlocks = 2;
            
            dataSize = widthBlocks * heightBlocks * ((blockSize  * bpp) / 8);
        }
        
    }
    
    
    FIBITMAP *dib = FreeImage_AllocateHeaderT(header_only, FIT_CUSTOM, header.width, header.height, header.bpp,
                                             FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);
    if (!dib) {
        FreeImage_OutputMessageProc(s_format_id, FI_MSG_ERROR_MEMORY);
        return NULL;
    }
    
    
    if (!header_only)
    {
        BYTE *bits = FreeImage_GetScanLine(dib, 0);
        if (io->read_proc(bits, 1, dataSize, handle) < dataSize)
        {
            FreeImage_OutputMessageProc(s_format_id, "Can't read data from file");
            return NULL;
        }
    }

    return dib;
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

// ==========================================================
//   Init
// ==========================================================

void DLL_CALLCONV
InitPVR(Plugin *plugin, int format_id) {
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


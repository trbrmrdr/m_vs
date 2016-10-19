#include "Helper.h"

using namespace App;

long Helper::GetLastDataEdit(const char* fileName) {
	struct tm* clock;
	struct stat attrib;
	stat(fileName, &attrib);
	clock = gmtime(&(attrib.st_mtime));
	return  clock->tm_hour*10000
		+clock->tm_min*100
		+clock->tm_sec;
}

const char* Helper::GLErrString(int err) {
	switch (err)
	{
		case GL_INVALID_ENUM:
			return "Invalid Enum";
		case GL_INVALID_VALUE:
			return "Invalid Value";
		case GL_INVALID_OPERATION:
			return "Invalid Operation";
		case GL_STACK_OVERFLOW:
			return "Stack overflow";
		case GL_STACK_UNDERFLOW:
			return "Stack underflow";
		case GL_OUT_OF_MEMORY:
			return "Out of memory";
		case 0x0506:
			return "Invalid framebufer operation";
	}
	return "";
}

void Helper::CheckGLError() {
	for (GLenum err; (err = glGetError())!=GL_NO_ERROR;)
	{
		std::cout<<"GL_ERROR: "<<err<<" "<<GLErrString(err)<<std::endl;
	}
}


GLuint Helper::LoadTexture(const std::string& fileName) {
	GLuint texture = 0;
	const char *filename = fileName.c_str();
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
	FIBITMAP* bitmap = FreeImage_Load(format, filename);
	if (NULL==bitmap)
	{
		Logger::Instance()->OutputString("Error: LoadFreeImage");
	}
	else
	{
		FIBITMAP *pImage = FreeImage_ConvertTo32Bits(bitmap);
		int nWidth = FreeImage_GetWidth(pImage);
		int nHeight = FreeImage_GetHeight(pImage);
		unsigned char *texels = FreeImage_GetBits(pImage);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, texels);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp->w, bmp->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		FreeImage_Unload(pImage);
	}
	return texture;

}

void Helper::SaveLastTexture(const std::string& fileName, size_t width, size_t height) {
	int bitsPerPixel = 32;//24
	int bytesPerPixel = (int)ceil(bitsPerPixel/8.0);
	unsigned char   *pixels = new unsigned char[(size_t)width*(size_t)height*(size_t)bitsPerPixel/8];

	glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);                                                        // set read non block aligned...
	glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
	glPopClientAttrib();

	int sizeOfOneLineOfPixels = width * bytesPerPixel;
	unsigned char * tempLineOfPix = new unsigned char[sizeOfOneLineOfPixels];
	unsigned char * linea;
	unsigned char * lineb;
	for (int i = 0; i<height/2; i++)
	{
		linea = pixels+i * sizeOfOneLineOfPixels;
		lineb = pixels+(height-i-1) * sizeOfOneLineOfPixels;
		memcpy(tempLineOfPix, linea, sizeOfOneLineOfPixels);
		memcpy(linea, lineb, sizeOfOneLineOfPixels);
		memcpy(lineb, tempLineOfPix, sizeOfOneLineOfPixels);
	}
	delete[] tempLineOfPix;
	//#############################
	FIBITMAP *bmp = FreeImage_ConvertFromRawBits(pixels, width, height, width*bytesPerPixel, bitsPerPixel, 0, 0, 0, true);
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(fileName.c_str(), 0);
	if (fif==FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(fileName.c_str());
	if ((fif!=FIF_UNKNOWN)&&FreeImage_FIFSupportsWriting(fif))
		FreeImage_Save(fif, bmp, fileName.c_str(), 0);
	if (bmp!=NULL)
		FreeImage_Unload(bmp);
}
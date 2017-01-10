#include "stdafx.h"
#include "Helper.h"

using namespace Helper;
using namespace Xml;
using namespace Strings;

vector<string> Helper::GetFilesInDirectory(const string &directory, const string& mask)
{
	vector<string> ret;
#if TARGET_WIN32
	HANDLE dir;
	WIN32_FIND_DATA file_data;

	if ((dir = FindFirstFile((directory + "/" + mask).c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return ret; /* No files found */

	do
	{
		const string file_name = file_data.cFileName;
		const string full_file_name = directory + "/" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		if (is_directory)
			continue;

		ret.push_back(full_file_name);
	}
	while (FindNextFile(dir, &file_data));

	FindClose(dir);
#else
	DIR *dir;
	class dirent *ent;
	class stat st;

	dir = opendir(directory);
	while ((ent = readdir(dir)) != NULL)
	{
		const string file_name = ent->d_name;
		const string full_file_name = directory + "/" + file_name;

		if (file_name[0] == '.')
			continue;

		if (stat(full_file_name.c_str(), &st) == -1)
			continue;

		const bool is_directory = (st.st_mode & S_IFDIR) != 0;

		if (is_directory)
			continue;

		ret.push_back(full_file_name);
	}
	closedir(dir);
#endif
	return ret;
} // GetFilesInDirectory

Vec2* Helper::getSplinePoint(const Vec2& origin, const Vec2& control1, const Vec2& control2, const Vec2& destination, unsigned int segments)
{

	Vec2* vertices = new (nothrow) Vec2[segments + 1];

	float t = 0;
	for (unsigned int i = 0; i < segments; i++)
	{
		vertices[i].x = powf(1 - t, 3) * origin.x + 3.0f * powf(1 - t, 2) * t * control1.x + 3.0f * (1 - t) * t * t * control2.x + t * t * t * destination.x;
		vertices[i].y = powf(1 - t, 3) * origin.y + 3.0f * powf(1 - t, 2) * t * control1.y + 3.0f * (1 - t) * t * t * control2.y + t * t * t * destination.y;
		t += 1.0f / segments;
	}
	vertices[segments].x = destination.x;
	vertices[segments].y = destination.y;
	return vertices;
}

long Helper::GetLastDataEdit(const char* fileName)
{
	struct tm* clock;
	struct stat attrib;
	stat(fileName, &attrib);
#if 0
	time_t rawtime;
	time(&rawtime);
	clock = gmtime(&rawtime);
#endif
	clock = gmtime(&(attrib.st_mtime));
	return  clock->tm_hour * 10000
		+ clock->tm_min * 100
		+ clock->tm_sec;
}

#include <time.h>
long Helper::GetCurrTime()
{
#if TARGET_WIN32
	SYSTEMTIME st;
	GetSystemTime(&st);
	return st.wHour * 10000000
		+ st.wMinute * 100000
		+ st.wSecond * 1000
		+ st.wMilliseconds;
#else
	timeval tv;
	gettimeofday(&tv, 0);
#endif
}

const char* Helper::GLErrString(int err)
{
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

void Helper::CheckGLError()
{
	for (GLenum err; (err = glGetError()) != GL_NO_ERROR;)
	{
		cout << "GL_ERROR: " << err << " " << GLErrString(err) << endl;
	}
}

GLuint Helper::LoadTexture(const string& fileName)
{
	GLuint texture = 0;
	const char *filename = fileName.c_str();
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
	FIBITMAP* bitmap = FreeImage_Load(format, filename);
	if (NULL == bitmap)
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

void Helper::SaveTexture(GLuint texture, const string& fileName, const Size& sizeWindow, const Vec2& pos, const Size& size)
{

	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	uint widthScreen = m_viewport[2]; // (uint) sizeWindow.width;
	uint heightScreen = m_viewport[3]; //(uint) sizeWindow.height;

	Vec2 p1 = Math::Clamp(pos, Vec2(), pos);
	Vec2  p2 = Math::Clamp(p1 + size, Vec2(), Vec2(widthScreen, heightScreen));

	uint width = p2.x;
	uint height = p2.y;

	uint bitsPerPixel = 32;//24
	uint bytesPerPixel = (uint) ceil(bitsPerPixel / 8.0);
	uchar   *pixels = new uchar[width * height * bitsPerPixel / 8];


	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	*width = m_viewport[2];
	*height = m_viewport[3];

	GLuint texture;
	glGenTextures(1, &amp; texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// rgb image
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_viewport[0],
					 m_viewport[1], m_viewport[2], m_viewport[3], 0);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	BYTE *raw_img = (BYTE*) malloc(sizeof(BYTE) * *width * *height * 3);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_img);

	return raw_frame;

	glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);                                                        // set read non block aligned...
	//glReadPixels(0, 0, widthScreen, heightScreen, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
	glReadPixels(p1.x, p1.y, width, height, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
	glPopClientAttrib();

	uint sizeOfOneLineOfPixels = width * bytesPerPixel;
	uchar * tempLineOfPix = new uchar[sizeOfOneLineOfPixels];
	uchar * linea;
	uchar * lineb;
	for (uint i = 0; i < height / 2; i++)
	{
		linea = pixels + i * sizeOfOneLineOfPixels;
		lineb = pixels + (height - i - 1) * sizeOfOneLineOfPixels;
		memcpy(tempLineOfPix, linea, sizeOfOneLineOfPixels);
		memcpy(linea, lineb, sizeOfOneLineOfPixels);
		memcpy(lineb, tempLineOfPix, sizeOfOneLineOfPixels);
	}
	delete[] tempLineOfPix;
	//#############################
	FIBITMAP *bmp = FreeImage_ConvertFromRawBits(pixels, width, height, width*bytesPerPixel, bitsPerPixel, 0, 0, 0, true);
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(fileName.c_str(), 0);
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(fileName.c_str());
	if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsWriting(fif))
		FreeImage_Save(fif, bmp, fileName.c_str(), 0);
	if (bmp != NULL)
		FreeImage_Unload(bmp);
}

long Helper::getFileDataHash(const char* fileName)
{
	FILE* fp = fopen(fileName, "rt");
	if (fp == NULL)
		return -1;
	long ret = Helper::GetLastDataEdit(fileName);
	fclose(fp);
	return ret;
}
//Xml

string Xml::XmlAttrToString(TiXmlElement* node, const string& name, const string& def)
{
	if (!node || name.empty()) return def;

	const char* val = node->Attribute(name.c_str());

	return val ? val : def;
}

int Xml::XmlAttrToInt(TiXmlElement* node, const string& name, const int def)
{
	if (!node || name.empty()) return def;

	const char* val = node->Attribute(name.c_str());

	return val ? StrToInt(val) : def;
}

bool Xml::XmlAttrToBool(TiXmlElement* node, const string& name, const bool def)
{
	return (bool) XmlAttrToInt(node, name, (bool) def);
}

float Xml::XmlAttrToFloat(TiXmlElement* node, const string& name, const float def)
{
	if (!node || name.empty()) return def;

	const char* val = node->Attribute(name.c_str());

	return val ? StrToFloat(val) : def;
}

/*
float Xml::XmlAttrToRndFloat(TiXmlElement* node, const string& name, const float def) {
	if (!node || name.empty()) return def;

	const char* val = node->Attribute(name.c_str());
	if (!val) return def;

	vector<string> parse;
	parse = SplitString(val, " ,");


	if (parse.size() >= 2)
	{
		return Math::Random(StrToFloat(parse[0]), StrToFloat(parse[1]));
	}
	else if (!parse.empty())
	{
		return StrToFloat(parse[0]);
	}

	return def;
}

Vec2 Xml::XmlTwoAttrToVec2(TiXmlElement* node, const string& name, Vec2 def) {
	def.x = Xml::XmlAttrToFloat(node, name + "_x", def.x);
	def.y = Xml::XmlAttrToFloat(node, name + "_y", def.y);

	return def;
}
*/
string Xml::XmlToString(TiXmlElement* node, const string& def)
{
	if (!node) return def;

	const char* val = node->GetText();

	return val ? val : def;
}

int Xml::XmlToInt(TiXmlElement* node, const int def)
{
	if (!node) return def;

	const char* val = node->GetText();

	return val ? StrToInt(val) : def;
}

bool Xml::XmlToBool(TiXmlElement* node, const bool def)
{
	return (bool) XmlToInt(node, (bool) def);
}

float Xml::XmlToFloat(TiXmlElement* node, const float def)
{
	if (!node) return def;

	const char* val = node->GetText();

	return val ? StrToFloat(val) : def;
}

Vec2 Xml::XmlToVec2(TiXmlElement* node, Vec2 def)
{
	if (!node) return def;

	const char* val = node->GetText();
	if (!val) return def;

	vector<string> parse;
	parse = SplitString(val, " ,");
	if (!parse.empty())
	{
		def.x = StrToFloat(parse[0]);
	}

	if (parse.size() >= 2)
	{
		def.y = StrToFloat(parse[1]);
	}

	return def;
}

TiXmlElement* Xml::XmlSetText(TiXmlElement* node, const string& val)
{
	if (!node || val.empty()) return node;

	TiXmlText* text = new TiXmlText(val);
	node->LinkEndChild(text);

	return node;
}

TiXmlElement* Xml::XmlSetTextFromInt(TiXmlElement* node, int val)
{
	return XmlSetText(node, IntToStr(val));
}

TiXmlElement* Xml::XmlSetTextFromFloat(TiXmlElement* node, float val)
{
	return XmlSetText(node, FloatToStr(val));
}

TiXmlElement* Xml::XmlSetTextFromBool(TiXmlElement* node, bool val)
{
	return XmlSetTextFromInt(node, val);
}

TiXmlElement* XmlSetTextFromPoint(TiXmlElement* node, const Vec2& val)
{
	string str = FormatToString("%f, %f", val.x, val.y);
	return Xml::XmlSetText(node, str);
}

TiXmlElement* XmlNewChild(TiXmlElement* node, const string& name)
{
	if (!node || name.empty()) return NULL;

	TiXmlElement* newNode = new TiXmlElement(name);
	node->LinkEndChild(newNode);
	return newNode;
}

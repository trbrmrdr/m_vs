#pragma once
#include "stdafx.h"
#include "xml/tinyxml.h"
using namespace std;

#define CHECK_GL_ERROR() Helper::CheckGLError()

namespace Helper {

	vector<string> GetFilesInDirectory(const string &directory,const string& mask = "*");

	std::string getDataDir();

	GLuint LoadTexture(const string& path_to_file);
	void SaveTexture(GLuint texture,const string& path_to_file, const Vec2& pos, const Size& size);
	const char* GLErrString(int err);
	void CheckGLError();
	long GetLastDataEdit(const char* path_to_file);
	long GetCurrTime();

	Vec2* getSplinePoint(const Vec2& origin, const Vec2& control1, const Vec2& control2, const Vec2& destination, unsigned int segments);

	long getFileDataHash(const char* path_to_file);

	namespace Xml {

		std::string XmlAttrToString(TiXmlElement* node, const std::string& name, const std::string& def);
		int XmlAttrToInt(TiXmlElement* node, const std::string& name, const int def);
		//Color::TColor XmlAttrToColor(TiXmlElement* node, const std::string& name, const Color::TColor def = Color::None);

		bool XmlAttrToBool(TiXmlElement* node, const std::string& name, const bool def);
		float XmlAttrToFloat(TiXmlElement* node, const std::string& name, const float def);
		//float XmlAttrToRndFloat(TiXmlElement* node, const std::string& name, const float def);
		//FPoint XmlTwoAttrToPoint(TiXmlElement* node, const std::string& name, FPoint def);
		Vec2 XmlAttrToVec2(TiXmlElement* node, const std::string& name, Vec2 def);
		//FRect XmlAttrToRect(TiXmlElement* node, const std::string& name, FRect def);

		std::string XmlToString(TiXmlElement* node, const std::string& def);
		int XmlToInt(TiXmlElement* node, const int def);
		bool XmlToBool(TiXmlElement* node, const bool def);
		float XmlToFloat(TiXmlElement* node, const float def);
		Vec2 XmlToVec2(TiXmlElement* node, Vec2 def);
		//FRect XmlToRect(TiXmlElement* node, FRect def);

		TiXmlElement* XmlSetText(TiXmlElement* node, const std::string& val);
		TiXmlElement* XmlSetTextFromInt(TiXmlElement* node, int val);
		TiXmlElement* XmlSetTextFromFloat(TiXmlElement* node, float val);
		TiXmlElement* XmlSetTextFromBool(TiXmlElement* node, bool val);
		TiXmlElement* XmlSetTextFromVec2(TiXmlElement* node, const Vec2& val);
		//TiXmlElement* XmlSetTextFromRect(TiXmlElement* node, const FRect& val);

		TiXmlElement* XmlNewChild(TiXmlElement* node, const std::string& name);
	}

};

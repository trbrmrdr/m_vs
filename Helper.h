#pragma once
#include "stdafx.h"

#define CHECK_GL_ERROR Helper::CheckGLError
class Helper {
public:
	static GLuint LoadTexture(const std::string& fileName);
	static void SaveLastTexture(const std::string& fileName, size_t width, size_t height);
	static const char* GLErrString(int err);
	static void CheckGLError();
	static long GetLastDataEdit(const char* fileName);
};

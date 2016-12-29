#pragma once
#include "stdafx.h"

using namespace std;
class ShaderGL {
private:
	bool OK;
	GLuint shaderProgram;

	set<int> errorLinesFS;
	set<int> errorLinesVS;
public:
	ShaderGL();
	ShaderGL(const string& filename);
	virtual ~ShaderGL();

	const set<int>& GetErrorLinesFS() { return errorLinesFS; }
	const set<int>& GetErrorLinesVS() { return errorLinesVS; }

	GLuint Compile(const string& fsshader);

	GLuint CompileFromFile(const string& filename);

	bool Valid();
	void Bind();
	void Unbind();
	void SetUniform(const GLchar* name, uint i);
	void SetUniform(const GLchar* name, int i);
	void SetUniform(const GLchar* name, float v);
	void SetUniform(const GLchar* name, float* fv, int size);
	void SetUniform(const GLchar* name, float x, float y);
	void SetUniform(const GLchar* name, const Vec2& vec2);

	void Free();
};


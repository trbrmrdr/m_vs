#include "stdafx.h"
#include "ShaderGL.h"

enum ShaderGLType
{
	GLSL_VS,
	GLSL_FS
};

void getErrorLog(GLuint shader, std::set<int>* errorLines = NULL)
{
	GLsizei bufSize = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize > 1)
	{
		GLchar *infoLog;
		GLsizei length;

		infoLog = new GLchar[bufSize];

		glGetShaderInfoLog(shader, bufSize, &length, infoLog);
		LOGGER()->OutputString("Compile Status: " + std::string(infoLog));

		// �K���ȉ��
		if (errorLines != NULL)
		{
			std::string tmpStr;
			for (int i = 0; i < bufSize; i++)
			{
				if (infoLog[i] == '\n')
				{
					int num = 0;
					int numcnt = 0;
					bool inNum = false;
					for (uint j = 0; j < tmpStr.length(); j++)
					{
						if (inNum)
						{
							if (isdigit(tmpStr[j]))
							{
								num = num * 10 + (tmpStr[j] - '0');
							}
							else
							{
								inNum = false;
								numcnt++;

								// ��Ԗڂ̐���
								if (numcnt == 2)
								{
									errorLines->insert(num);
								}
							}
						}
						else
						{
							if (isdigit(tmpStr[j]))
							{
								inNum = true;
								num = num * 10 + (tmpStr[j] - '0');
							}
						}
					}
					tmpStr = "";
				}
				else
				{
					tmpStr += infoLog[i];
				}
			}
		}

		delete[] infoLog;
	}
}

GLuint CompileShader(ShaderGLType type, const GLchar* source, std::set<int>* errorLines = NULL)
{
	GLint status;
	unsigned int prog = 0;
	switch (type)
	{
		case GLSL_VS:
		{
			prog = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(prog, 1, &source, 0);
			glCompileShader(prog);
			glGetShaderiv(prog, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE)
			{
				getErrorLog(prog, errorLines);
				LOGGER()->OutputString("Compile error in vertex shader.");
				glDeleteShader(prog);
				prog = 0;
			}
			return prog;
		}break;
		case GLSL_FS:
		{
			prog = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(prog, 1, &source, 0);
			glCompileShader(prog);
			glGetShaderiv(prog, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE)
			{
				getErrorLog(prog, errorLines);
				LOGGER()->OutputString("Compile error in fragment shader.");
				glDeleteShader(prog);
				prog = 0;
			}

			return prog;
		}break;
	}
	return 0;
}

GLuint LinkShader(GLuint vsh, GLuint fsh)
{
	GLuint program = 0;
	if (vsh != 0 && fsh != 0)
	{
		program = glCreateProgram();
		glAttachShader(program, vsh);
		glAttachShader(program, fsh);
		// �����N
		glLinkProgram(program);
		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			LOGGER()->OutputString("Link Error.");
			glDeleteProgram(program);
			program = 0;
		}
	}

	return program;
}

ShaderGL::ShaderGL(const string& filename) :
	fs_filename(filename)
{
	OK = false;
	shaderProgram = 0;
	//if (0 == CompileFromFile(filename))
	//	LOGF("shader %s compile error",filename.c_str());
}

/*
ShaderGL::ShaderGL()
{
	OK = false;
	shaderProgram = 0;
}
*/

std::string readFile(const std::string& filename)
{
	std::string ret;
	std::string t_pathToFile = Helper::getDataDir() + filename;
	FILE* fp = fopen(t_pathToFile.c_str(), "rt");
	if (fp != NULL)
	{
		char buf[1024];
		while (fgets(buf, 1024, fp) != NULL)
		{
			ret += buf;
		}

		fclose(fp);
	}
	return ret;
}

GLuint ShaderGL::CompileFromFile(const string& filename)
{
	fs_filename = string(filename);
	OK = false;
	shaderProgram = 0;
	string fsshader = readFile(fs_filename);
	if (!fsshader.empty())
		return Compile(fsshader);
	return 0;
}

GLuint ShaderGL::Compile(const string& fsshader)
{
	// ���_�V�F�[�_�[�i�Œ�j
	std::string vsshader = "\
			attribute vec2 pos;\
			\
			//attribute vec2 a_vertex_xy;\
			\
			attribute vec2 a_vertex_uv;\
			varying vec2 v_uv;\
			\
			void main(){\
				gl_Position=vec4(pos.xy,0,1);\
				/*gl_Position=vec4(a_vertex_xy,0,1);*/\
				\
				/*v_uv = a_vertex_uv;*/\
			}";
	CHECK_GL_ERROR();
	vsshader = readFile("base.glvs");
	errorLinesVS.clear();
	GLuint vsh = CompileShader(GLSL_VS, vsshader.c_str(), &errorLinesVS);
	if (vsh == 0)
	{
		LOGGER()->OutputString("Vertex Shader Error.");
		return 0;
	}
	CHECK_GL_ERROR();
	errorLinesFS.clear();
	GLuint fsh = CompileShader(GLSL_FS, fsshader.c_str(), &errorLinesFS);
	if (fsh == 0)
	{
		LOGGER()->OutputString("Fragment Shader Error.");
		glDeleteShader(vsh);
		return 0;
	}
	CHECK_GL_ERROR();
	GLuint program = LinkShader(vsh, fsh);
	if (program != 0)
	{
		OK = true;
		if (shaderProgram != 0)
			glDeleteProgram(shaderProgram);
		shaderProgram = program;

		glDeleteShader(vsh);
		glDeleteShader(fsh);
	}
	else
		return 0;
	//Bind();
	CHECK_GL_ERROR();
	// �V�����V�F�[�_�[�v���O�����Z�b�g���ꂽ�炱���ɂ���
	return shaderProgram;
}

ShaderGL::~ShaderGL()
{
	Free();
}

void ShaderGL::Free()
{
	if (shaderProgram != 0)
	{
		glDeleteProgram(shaderProgram);
		shaderProgram = 0;
	}
}

bool ShaderGL::Valid()
{
	return OK;
}

void ShaderGL::Bind()
{
	glUseProgram(shaderProgram);
}

void ShaderGL::Unbind()
{
	glUseProgram(0);
}

void ShaderGL::SetUniform(const GLchar* name, uint i)
{
	if (shaderProgram)
	{
		GLuint id = glGetUniformLocation(shaderProgram, name);
		if (id != -1)
			glUniform1i(id, i);
	}
}

void ShaderGL::SetUniform(const GLchar* name, int i)
{
	if (shaderProgram)
	{
		GLuint id = glGetUniformLocation(shaderProgram, name);
		if (id != -1)
			glUniform1i(id, i);
	}
}

void ShaderGL::SetUniform(const GLchar* name, float v)
{
	if (shaderProgram)
	{
		GLuint id = glGetUniformLocation(shaderProgram, name);
		if (id != -1)
			glUniform1f(id, v);
	}
}

void ShaderGL::SetUniform(const GLchar* name, float* fv, int size)
{
	if (shaderProgram)
	{
		GLuint id = glGetUniformLocation(shaderProgram, name);
		if (id != -1)
			glUniform1fv(id, size, fv);
	}
}

void ShaderGL::SetUniform(const GLchar* name, float x, float y)
{
	if (shaderProgram)
	{
		GLuint id = glGetUniformLocation(shaderProgram, name);
		if (id != -1)
			glUniform2f(id, x, y);
	}
}

void ShaderGL::SetUniform(const GLchar* name, const Vec2& vec2)
{
	if (shaderProgram)
	{
		GLuint id = glGetUniformLocation(shaderProgram, name);
		if (id != -1)
			glUniform2f(id, vec2.x, vec2.y);
	}
}

void ShaderGL::SetUniform(const GLchar* name, const Vec3& vec3)
{
	if (shaderProgram)
	{
		GLuint id = glGetUniformLocation(shaderProgram, name);
		if (id != -1)
			glUniform3f(id, vec3.x, vec3.y, vec3.z);
	}
}

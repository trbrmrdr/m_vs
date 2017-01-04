#pragma once
#include "stdafx.h"
using namespace std;

#include "file.h"
#include "GlobalSettings.h"


class Scene
{
private:
	int frame;
	ShaderGL* shaderGL;
	map<string, string>	channels;
	map<string, GLuint> iTextureChannels;
public:
	Scene(const string& fs_filename, map<string, string> channels) :frame(0)
	{
		shaderGL = new ShaderGL(fs_filename);
		this->channels = channels;
	}

	~Scene() { free(); freeChannels(); delete shaderGL; }

	ShaderGL* getSahder() { return shaderGL; }

	bool isValid() { return shaderGL->Valid(); }

	void draw(vector<void*> data, float realSec);

	void free()
	{
		frame = 0;
		shaderGL->Unbind();
	}

	void load()
	{
		if (!shaderGL->Valid())
			shaderGL->Compile();
	}

	void freeChannels()
	{
		for (auto it : iTextureChannels)
			glDeleteTextures(1, &it.second);
		iTextureChannels.clear();
	}

	void loadChannels()
	{
		for (auto it : channels)
		{
			//if (iTextureChannels.find(it.second) == iTextureChannels.end())
			{
				GLuint newTetxture = Helper::LoadTexture(it.second);
				iTextureChannels.insert(pair<string, GLuint>(it.first, newTetxture));
			}
		}
	}
};
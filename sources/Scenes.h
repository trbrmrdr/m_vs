#pragma once
#include "stdafx.h"

using namespace std;

struct file {
	string fileName;
	long hash = 0;

	bool checkHash() {
		long ret = Helper::GetLastDataEdit(fileName.c_str());
		hash = ret;
		if (ret != hash)
			return false;
	}

	file() {}
	file(const string& _fileName):fileName(_fileName) {
		checkHash();
	}
};

struct Setting {
	file file_vs;
	file file_fs;

	string keyVal;
	file optionTexture;

	Setting() {}
	Setting(const string& _file_vs, const string& keyVal) {}
};




#define OPTION_TEXTURE "option.png"

struct EffectLoadCallback {
	EffectLoadCallback() {};
	~EffectLoadCallback() {};
	virtual void loadEffectCallback(uint effectId) {};
};

class Scenes {
	bool isFree;
	file settingsFile;
	Setting global_setting;
	//optTex

	// FBO
	GLuint frameBuffer;
	GLuint renderBuffer;
	GLuint renderTexture;

	GLuint backTexture;

	// option texture
	GLuint optionTexture;

	std::map<std::string, long> hashEffectFileTable;

	vector<ShaderGL*> shaderGL;
	bool needSaveFronBuf;
	int pEffectId;
	int nowEffect;

	EffectLoadCallback* callback;
public:
	void setEffectLoadCallback(EffectLoadCallback* _callback) {
		callback = _callback;
	}

	Scenes(const string& fileName):
		settingsFile(fileName),
		needSaveFronBuf(false),
		pEffectId(-1),
		nowEffect(0),
		isFree(true) {};

	~Scenes() { free(); }

	void init();

	void free() {
		isFree = true;
		for (auto it : shaderGL)
		{
			delete it;
		}

		if (frameBuffer != 0)
			glDeleteFramebuffers(1, &frameBuffer);
		if (renderBuffer != 0)
			glDeleteRenderbuffers(1, &renderBuffer);
		if (renderTexture != 0)
			glDeleteTextures(1, &renderTexture);
		if (backTexture != 0)
			glDeleteTextures(1, &backTexture);
		if (optionTexture != 0)
			glDeleteTextures(1, &optionTexture);

		frameBuffer = 0;
		renderBuffer = 0;
		renderTexture = 0;
		backTexture = 0;
		optionTexture = 0;
	}

	void load(uint effectId);

	bool nowIsValid() {
		return shaderGL[nowEffect]->Valid();
	}

	void swapRenderTarget() {
		// Swap render target
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	}

	void clear() {
		CHECK_GL_ERROR();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
	}

	void begin(float realSec);

	void end();

	void draw();
	/*
	void Scenes::checkValidHashFile() {
	const char* file = EffectFileTable[nowEffect];
	std::map<std::string, long>::iterator it = hashEffectFileTable.find(std::string(file));
	if (it != hashEffectFileTable.end())
	{
		long newD = Helper::GetLastDataEdit(file);
		if (newD != it->second)
		{
			it->second = newD;
			loadEffect(nowEffect);
			return;
		}
	}
	else
	{
		long hash = Helper::getFileDataHash(file);
		if (hash != -1)
			hashEffectFileTable.insert(std::pair<std::string, long>(file, hash));
	}
}
*/
	void update() {
		//	checkValidHashFile();
	}

	int changeKeys(SDL_Keysym keysym) {
		int ctrl = keysym.mod & KMOD_CTRL;
		int alt = keysym.mod & KMOD_ALT;

		/*
		if (SDLK_F1 <= keysym.sym && keysym.sym <= SDLK_F12 && keysym.sym != SDLK_F11)
		{
			int nowEff = keysym.sym - SDLK_F1;
			if (alt)
				nowEff += 12;
			else if (ctrl)
				nowEff += 24;

			if (nowEffect != nowEff)
			{
				nowEffect = nowEff;
				loadEffect(nowEffect);
			}
			return 1;
		}

		if (keysym.sym == SDLK_F11)
		{
			if (editMode)
				editMode = false;
			else
				editMode = true;
			return 1;
		}

		if (keysym.mod & KMOD_CTRL && keysym.sym == SDLK_TAB)
		{
			errorHighlight = !errorHighlight;
			return 1;
		}

		if (keysym.sym == SDLK_q)
		{
			saveFronBuf = true;
			return 1;
		}
		*/
		return 1;
	}

};

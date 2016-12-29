#pragma once
#include "stdafx.h"

using namespace std;

struct file
{
	string fileName;
	long isEditData = 0;

	bool checkIsEdit()
	{
		long new_data = Helper::GetLastDataEdit(fileName.c_str());
		bool ret = new_data != isEditData;
		isEditData = new_data;
		return ret;
	}

	file() {}
	file(const string& _fileName) :fileName(_fileName)
	{
		checkIsEdit();
	}
};

struct Setting
{
	file file_vs;
	file file_fs;

	string keyVal;
	file optionTexture;

	Setting() {}
	Setting(const string& _file_vs, const string& keyVal) {}
};




struct EffectLoadCallback
{
	EffectLoadCallback() {};
	~EffectLoadCallback() {};
	virtual void loadEffectCallback(uint effectId) {};
};

class Scenes
{
	struct GlobalSettings
	{
		file settingsFile;
		file optionTexture;
		string maskSaveText;
		Vec2 posSave;
		Size sizeSave;
		GlobalSettings(const string& fileName) :
			settingsFile(fileName),
			posSave(),
			sizeSave(-1, -1),
			optionTexture("option.png"),
			maskSaveText("save_text_")
		{}
		bool checkEdit() { return isEmpty() && settingsFile.checkIsEdit(); }
		bool isEmpty() { return settingsFile.isEditData == 0; }

		void save() {};
		void parse() {};


		string getNewNameSavesText()
		{
			string mask = Strings::FormatToString("%s*.png", maskSaveText.c_str());
			vector<string> files = Helper::GetFilesInDirectory(".", mask);
			int id = 0;
			int new_id = 0;
			for (vector<string>::iterator it = files.begin(), it_e = files.end(); it != it_e; ++it)
			{
				string file = Strings::ToLower(*it);
				file = file.erase(0, maskSaveText.size() + 2);//"./mask...."
				file = file.erase(file.find(".png"), 4);
				int tmp = Strings::StrToInt(file);
				if (id != new_id)
					new_id = tmp;
				else
				{
					id++;
					new_id++;
				}
			}
			do
			{
				string ret = Strings::FormatToString("%s%i.png", maskSaveText.c_str(), new_id);
				if (Helper::GetLastDataEdit(ret.c_str()) == 0)
					return ret;
				new_id++;
			}
			while (true);
			//return ;
		}

	} globalSettings;


	bool isFree;
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
	void setEffectLoadCallback(EffectLoadCallback* _callback)
	{
		callback = _callback;
	}

	Scenes(const string& fileName) :
		globalSettings(fileName),
		needSaveFronBuf(false),
		pEffectId(-1),
		nowEffect(0),
		isFree(true)
	{};

	~Scenes() { free(); }

	void init();

	void free()
	{
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

	bool nowIsValid()
	{
		return shaderGL[nowEffect]->Valid();
	}

	void swapRenderTarget()
	{
		// Swap render target
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	}

	void clear()
	{
		CHECK_GL_ERROR();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
	}

	void begin(float realTime);

	void end();

	void draw();

	void update();

	int changeKeys(SDL_Keysym keysym)
	{
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

	void parseSettings();
};

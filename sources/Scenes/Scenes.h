#pragma once
#include "stdafx.h"
using namespace std;

#include "file.h"
#include "GlobalSettings.h"
#include "Scene.h"

struct EffectLoadCallback
{
	EffectLoadCallback() {};
	~EffectLoadCallback() {};
	virtual void loadEffectCallback(uint effectId) {};
};

class Scenes
{
	GlobalSettings globalSettings;

	bool isFree;
	//optTex

	// FBO
	GLuint frameBuffer;
	GLuint renderBuffer;
	GLuint renderTexture;

	GLuint backTexture;
	GLuint texture3;

	// option texture
	GLuint optionTexture;

	map<int, Scene*> scenes;
	Scene* nowScene;

	bool needSaveFronBuf;
	int nowEffectId;
	bool space_press;
	bool mouse_left_press;

	EffectLoadCallback* callback;
public:
	void setEffectLoadCallback(EffectLoadCallback* _callback)
	{
		callback = _callback;
	}

	Scenes(const string& fileName) :
		globalSettings(fileName),
		needSaveFronBuf(false),
		nowEffectId(1),
		isFree(true),
		space_press(false),
		nowScene(NULL)
	{};

	void free();
	~Scenes() { free(); }

	void init(bool force);

	bool isValid() { return NULL != nowScene&& nowScene->isValid(); }

	void draw(float realSec);

	void update();

	int changeKeys(const Uint8 *state, bool press);
private:
	bool parseSettings(bool force);

	void load(uint effectId);
	void drawScene(uint effectId);
	void saveNeeded();

};

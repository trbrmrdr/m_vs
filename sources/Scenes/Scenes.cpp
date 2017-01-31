#include "stdafx.h"
#include "Scenes.h"
#include "Core.h"
#include "MouseBuffer.h"

#define windowWidth			(float)CORE().getWindowSize().width
#define windowHeight		(float)CORE().getWindowSize().height

bool Scenes::processSettings(bool reload) {
	if (globalSettings.settingsFile.isEditData == 0)
	{
		globalSettings.readEmptySetting();
		globalSettings.save();
	}
	else
	{
		reload = globalSettings.read(reload);
	}
	globalSettings.load(scenes, reload);
	return reload;
}

void Scenes::init(bool force) {
	if (force)processSettings(true);
	if (!isFree)free();

	int width = windowWidth;
	int height = windowHeight;

	glViewport(0, 0, width, height);

	optionTexture = Helper::LoadTexture(globalSettings.optionTexture.fileName);
	texture3 = Helper::LoadTexture("./save_text_4.png");
	// Initialize back buffer
	backTexture = 0;
	glGenTextures(1, &backTexture);
	glBindTexture(GL_TEXTURE_2D, backTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Initialize frame buffer
	renderTexture = 0;
	glGenTextures(1, &renderTexture);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	frameBuffer = 0;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);

	// Initialize render buffer
	renderBuffer = 0;
	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);


	// Initialize render texture
	glBindTexture(GL_TEXTURE_2D, renderTexture);		//old 0
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);	//old 0
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);		//old 0

	CHECK_GL_ERROR();
	load(nowEffectId);
	CHECK_GL_ERROR();
}

void Scenes::draw(float realSec) {
	//## Swap render target
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	//## Clear
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//## Begin

	if (NULL == nowScene)
		return;

	if (!nowScene->isValid())
		return;
	ShaderGL* nowShader = nowScene->getSahder();

	vector<void*> data{(void*) space_press,(void*) backTexture,(void*) backTexture};


	long time = Helper::GetCurrTime();

	nowScene->draw(data, realSec);
#if 0 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glRecti(1, 1, -1, -1);
	//glBindTexture(GL_TEXTURE_2D, 0);

	nowShader->Unbind();

	LOGF("%i", Helper::GetCurrTime() - time);
	return;
#endif
	nowShader->Bind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glRecti(1, 1, -1, -1);
	glBindTexture(GL_TEXTURE_2D, 0);

	nowShader->Unbind();


	saveNeeded();
	//LOGF("%i", Helper::GetCurrTime() - time);

#if OLD
	if (!isValid())
		return;
	shaderGL[nowEffect]->Bind();

	shaderGL[nowEffect]->SetUniform("iResolution", CORE().getWindowSize());
	shaderGL[nowEffect]->SetUniform("iGlobalTime", realSec);
	shaderGL[nowEffect]->SetUniform("iFrame", frame);
	shaderGL[nowEffect]->SetUniform("iMouse", Vec3(MOUSE().getCursorPos(), MOUSE().LeftIsPress()));
	shaderGL[nowEffect]->SetUniform("press", (int) space_press);

	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, backTexture);
	shaderGL[nowEffect]->SetUniform("backbuffer", 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, optionTexture);
	shaderGL[nowEffect]->SetUniform("optTex", 2);


	//glActiveTexture(GL_TEXTURE3);
	//glBindTexture(GL_TEXTURE_2D, texture3);
	//shaderGL[nowEffect]->SetUniform("iChannel0", 3);

	int i = 0;
	for (auto it : iTextureChannels)
	{
		i++;
		glActiveTexture(GL_TEXTURE2 + i);
		glBindTexture(GL_TEXTURE_2D, it.second);
		shaderGL[nowEffect]->SetUniform(it.first.c_str(), 2 + i);
	}
	//## End

	glRecti(1, 1, -1, -1);
	//shaderGL[nowEffect]->Unbind();

	//glActiveTexture(GL_TEXTURE1);
	///glBindTexture(GL_TEXTURE_2D, 0);

	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, 0);

	//glActiveTexture(GL_TEXTURE3);
	//glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	// Copy render texture to back buffer texture
	glBindTexture(GL_TEXTURE_2D, backTexture);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, windowWidth, windowHeight, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);

	saveNeeded();

	//## Draw
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#if 0
	uint POSTFxID = 1;
	//return;
	//if (shaderGL[POSTFxID]->Valid())
	if (false)
	{
		CHECK_GL_ERROR();
		shaderGL[POSTFxID]->Bind();

		CHECK_GL_ERROR();
		glBindTexture(GL_TEXTURE_2D, renderTexture);
		CHECK_GL_ERROR();
		shaderGL[POSTFxID]->SetUniform("texture0", renderTexture);
		shaderGL[POSTFxID]->SetUniform("resolution", windowWidth, windowHeight);
		shaderGL[POSTFxID]->SetUniform("time", realSec);
		shaderGL[POSTFxID]->SetUniform("mouse", MOUSE());
		//shaderGL[POSTFxID]->SetUniform("lowFreq", Math::Random<float>());
		//shaderGL[POSTFxID]->SetUniform("midFreq", Math::Random<float>());
		//shaderGL[POSTFxID]->SetUniform("highFreq", Math::Random<float>());
		//shaderGL[POSTFxID]->SetUniform("editorCursorY", (float) cy);

		//float cy = (textEditor.GetCursorPosition().col - textEditor.GetLineOffset()) * 11

		glRecti(1, 1, -1, -1);
		glBindTexture(GL_TEXTURE_2D, 0);

		shaderGL[POSTFxID]->Unbind();
	}
	else
#endif



	{

		glBindTexture(GL_TEXTURE_2D, renderTexture);
		glRecti(1, 1, -1, -1);
		glBindTexture(GL_TEXTURE_2D, 0);
		shaderGL[pEffectId]->Unbind();
		//CHECK_GL_ERROR();
}
#endif
}

void Scenes::drawScene(uint effectId) {}

void Scenes::load(uint effectId) {
	if (NULL != nowScene)
	{
		nowScene->free();
		nowScene->freeChannels();
		nowScene = NULL;
	}

	nowScene = scenes[nowEffectId];

	nowScene->load();
	nowScene->loadChannels();

	LOGF("curr Effect = %i - %s", effectId, globalSettings.sceneSettings[nowEffectId].file_vs.fileName.c_str());
	//if (NULL != callback)
	//	callback->loadEffectCallback(effectId);
}

int Scenes::changeKeys(const Uint8 *state, bool press) {
	space_press = state[SDL_SCANCODE_SPACE];
	bool alt = state[SDL_SCANCODE_LALT] || state[SDL_SCANCODE_RALT];
	bool ctrl = state[SDL_SCANCODE_LCTRL] || state[SDL_SCANCODE_RCTRL];

	for (int i = 0; i < SDL_SCANCODE_F12; ++i)
	{
		if (state[SDL_SCANCODE_F1 + i])
		{
			int nowEff = i + 1;
			if (ctrl)
				nowEff += 12;
			else if (alt)
				nowEff += 24;

			if (nowEffectId != nowEff)
			{
				auto it = scenes.find(nowEff);
				if (it == scenes.end())
				{
					LOGF("scene %i not loaded", nowEff);
					return 1;
				}
				nowEffectId = nowEff;
				load(nowEffectId);
			}
			return 1;
		}
	}

	/*
	if (keysym.sym == SDLK_F11)
	{
		if (editMode)
			editMode = false;
		else
			editMode = true;
		return 1;
	}

	if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_TAB])
	{
		errorHighlight = !errorHighlight;
		return 1;
	}
	*/
	if (state[SDL_SCANCODE_Q])
	{
		needSaveFronBuf = true;
		return 1;
	}
	return 1;
}

void Scenes::update() {
	if (processSettings(false))
	{
		init(false);
	}
}

void Scenes::saveNeeded() {
	if (!needSaveFronBuf)
		return;
	needSaveFronBuf = false;
	Size sizeSave = globalSettings.sizeSave;
	if (globalSettings.sizeSave.width <= -1.f && globalSettings.sizeSave.height <= -1.f)
		sizeSave = CORE().getWindowSize();

	Helper::SaveTexture(renderTexture,
						globalSettings.getNewNameSavesText(),
						globalSettings.posSave,
						sizeSave);
}

void Scenes::free() {
	isFree = true;
	nowScene = NULL;
	for (auto it : scenes)
		delete it.second;
	scenes.clear();

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
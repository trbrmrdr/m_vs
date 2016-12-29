#include "stdafx.h"
#include "Scenes.h"
#include "Core.h"
#include "MouseBuffer.h"

#define windowWidth			(float)CORE().getWindowSize().width
#define windowHeight		(float)CORE().getWindowSize().height

void Scenes::parseSettings()
{
	if (globalSettings.isEmpty())
		globalSettings.save();
	else
		globalSettings.parse();
}

void Scenes::init()
{
	// parse settings
	parseSettings();
	if (!isFree)free();

	int width = windowWidth;
	int height = windowHeight;

	glViewport(0, 0, width, height);

	optionTexture = Helper::LoadTexture(globalSettings.optionTexture.fileName);

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
	load(nowEffect);
	CHECK_GL_ERROR();
}

static float sec = 0;
void Scenes::begin(float realTime)
{
	if (!nowIsValid())return;
	sec = realTime;
	CHECK_GL_ERROR();
	shaderGL[nowEffect]->Bind();
	CHECK_GL_ERROR();
	int width = windowWidth;
	int height = windowHeight;
	shaderGL[nowEffect]->SetUniform("resolution", width, height);
	shaderGL[nowEffect]->SetUniform("time", realTime);
	shaderGL[nowEffect]->SetUniform("mouse", MOUSE().getCursorPos());

	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, optionTexture);
	shaderGL[nowEffect]->SetUniform("optTex", optionTexture);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, backTexture);
	shaderGL[nowEffect]->SetUniform("backbuffer", backTexture);
}

void Scenes::end()
{
	if (!nowIsValid())return;

	glRecti(1, 1, -1, -1);
	//shaderGL[nowEffect]->Unbind();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);

	// Copy render texture to back buffer texture
	glBindTexture(GL_TEXTURE_2D, backTexture);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, windowWidth, windowHeight, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (needSaveFronBuf)
	{
		needSaveFronBuf = false;
		Size sizeSave = globalSettings.sizeSave;
		if (globalSettings.sizeSave.width <= -1.f && globalSettings.sizeSave.height <= -1.f)
			sizeSave = CORE().getWindowSize();

		Helper::SaveLastTexture(globalSettings.getNewNameSavesText(),
								CORE().getWindowSize(),
								globalSettings.posSave,
								sizeSave);
	}
}

void Scenes::draw()
{
	uint POSTFxID = 1;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
		shaderGL[POSTFxID]->SetUniform("time", sec);
		shaderGL[POSTFxID]->SetUniform("mouse", MOUSE().GetCursorX(), MOUSE().GetCursorY());
		//shaderGL[POSTFxID]->SetUniform("lowFreq", Math::Random<float>());
		shaderGL[POSTFxID]->SetUniform("midFreq", Math::Random<float>());
		//shaderGL[POSTFxID]->SetUniform("highFreq", Math::Random<float>());
		//shaderGL[POSTFxID]->SetUniform("editorCursorY", (float) cy);

		//float cy = (textEditor.GetCursorPosition().col - textEditor.GetLineOffset()) * 11

		glRecti(1, 1, -1, -1);
		glBindTexture(GL_TEXTURE_2D, 0);

		shaderGL[POSTFxID]->Unbind();
	}
	else
	{

		glBindTexture(GL_TEXTURE_2D, renderTexture);
		glRecti(1, 1, -1, -1);
		glBindTexture(GL_TEXTURE_2D, 0);
		shaderGL[pEffectId]->Unbind();
		CHECK_GL_ERROR();
	}
}

void Scenes::load(uint effectId)
{
	if (pEffectId != -1)
		shaderGL[pEffectId]->Unbind();
	pEffectId = (int) effectId;

	//shaderGL[pEffectId].CompileFromFile(EffectFileTable[effectId]);
	shaderGL.clear();
	//shaderGL.push_back(new ShaderGL("effect.glsl"));
	shaderGL.push_back(new ShaderGL("scene1.glsl"));
	if (NULL != callback)
		callback->loadEffectCallback(effectId);
}

void Scenes::update()
{
	if (globalSettings.checkEdit())
	{
		init();
	}
}
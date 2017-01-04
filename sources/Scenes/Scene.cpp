#include "stdafx.h"
#include "Scene.h"
#include "Core.h"
#include "MouseBuffer.h"

#define windowWidth			(float)CORE().getWindowSize().width
#define windowHeight		(float)CORE().getWindowSize().height





void Scene::draw(vector<void*> data, float realSec)
{
	//vector<void*> data{ (void*) space_press,(void*) backTexture,(void*) backTexture };
	bool space_press = (bool) data[0];
	GLuint backTexture = (GLuint) data[1];
	GLuint optionTexture = (GLuint) data[2];

	shaderGL->Bind();
	shaderGL->SetUniform("iResolution", CORE().getWindowSize());
	shaderGL->SetUniform("iGlobalTime", realSec);
	shaderGL->SetUniform("iFrame", frame);
	shaderGL->SetUniform("iMouse", Vec3(MOUSE().getCursorPos(), MOUSE().LeftIsPress()));

	shaderGL->SetUniform("press", (int) space_press);

	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, backTexture);
	shaderGL->SetUniform("backbuffer", 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, optionTexture);
	shaderGL->SetUniform("optTex", 2);

	frame++;
}
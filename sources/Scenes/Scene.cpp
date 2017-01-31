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

	int i = 0;
	for (auto it : iTextureChannels)
	{
		i++;
		glActiveTexture(GL_TEXTURE2 + i);
		glBindTexture(GL_TEXTURE_2D, it.second);
		shaderGL->SetUniform(it.first.c_str(), 2 + i);
	}

	glRecti(1, 1, -1, -1);
	shaderGL->Unbind();

	/*
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);
	*/

	glActiveTexture(GL_TEXTURE0);
	// Copy render texture to back buffer texture
	glBindTexture(GL_TEXTURE_2D, backTexture);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, windowWidth, windowHeight, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//saveNeeded();

#if 0
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glRecti(1, 1, -1, -1);
	glBindTexture(GL_TEXTURE_2D, 0);
#endif
	//shaderGL->Unbind();

	frame++;
}
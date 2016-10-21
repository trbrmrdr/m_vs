#pragma once
#include "stdafx.h"

namespace App
{

	const int EffectNum = 14;

	class Scene1
	{
	private:
		int FPS;
		int width;
		int height;

		std::map<std::string, long> hashEffectFileTable;

		bool saveFronBuf;
		bool editMode;
		bool nowCompiled;
		bool errorHighlight;

		int nowEffect;


		KeyBuffer keyBuffer;
		MouseBuffer mouseBuffer;
		ShaderGL shaderGL[EffectNum];
		//	ShaderGL postEffect;
		TextEditor textEditor;
		KeyAnalyzer keyAnalyzer;
		
		sound_system_c sound_system;

		std::string nowSource;

		Uint32 baseTime;
		Uint32 prevTime;
		int fps;

		float* audio_spectr_l = NULL;
		float* audio_spectr_r = NULL;
		GLuint audioTexture;

		// FBO
		GLuint frameBuffer;
		GLuint renderBuffer;
		GLuint renderTexture;

		GLuint backTexture;

		// option texture
		GLuint optionTexture;
	public:

		int init(int width_, int height_);
		void free();

		int Update(Uint32 nowTime);
		void render(float realSec);
		void changeMouse(int mouseX, int mouseY);
		int changeKeys(SDL_keysym keysym);
		void loadEffect(int effectId);

		void initHashFile();
		void checkValidHashFile();

		Scene1();
		~Scene1();
	};

};

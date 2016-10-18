#ifndef _CORE_H_
#define _CORE_H_

#include <SDL.h>

#ifdef __WIN32__
#define __NEEDSGLEW__
#endif __WIN32__

#ifdef __linux__
#define __NEEDSGLEW__
#endif __linux__

#ifdef	__WIN32__
#include <windows.h>
#endif	__WIN32__

#ifdef	__APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <string>
#include <math.h>

#include "Logger.h"
#include "KeyBuffer.h"
#include "MouseBuffer.h"
#include "ShaderGL.h"
#include "BitmapFontGL.h"
#include "TextEditor.h"
#include "KeyAnalyzer.h"
//#include "AudioAnalyzer.h"
#include "sound_system.h"

namespace LiveCoder {
	
const int DefaultWidth = 800;
const int DefaultHeight = 600;

const int EffectNum = 64;


class Core {
private:
	int width;
	int height;

	//int widthM;
	///int heightM;
	SDL_Surface* mSDL_Surface;
	int bpp;
	bool end;

	int SDLflags;
	bool fullScreen;
	bool saveFronBuf;
	bool editMode;
	bool nowCompiled;
	bool errorHighlight;

	int nowEffect;

	std::string title;

	int ProcessSDLEvents();

	KeyBuffer keyBuffer;
	MouseBuffer mouseBuffer;
	ShaderGL shaderGL[EffectNum];
//	ShaderGL postEffect;
	TextEditor textEditor;
	KeyAnalyzer keyAnalyzer;
	//AudioAnalyzer *audioAnalyzer;
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

	int Initialize(std::string title_, int width_, int height_, bool fullScreen_);
	int init();
	void deinit();

	int MainLoop();
	void Render();

	Core();
	virtual ~Core();
};

};

#endif

#pragma once
#include "stdafx.h"

class Application {
private:
	Size maxSize;
	Size startSize;
	Size currSize;

	SDL_Window* mSDL_Window;
	int bpp;
	bool end;

	int SDLflags;
	bool fullScreen;

	std::string title;

	int ProcessSDLEvents();
public:

	int Initialize(std::string title_, int width_, int height_, bool fullScreen_);

	int init();
	void free();

	int MainLoop();

	Application();
	virtual ~Application();
};

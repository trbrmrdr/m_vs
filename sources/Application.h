#pragma once
#include "stdafx.h"

class Application
{
private:
	vector<Size> maxSizeDisplay;
	Size startSize;
	Size currSize;

	SDL_Window* mSDL_Window;
	int bpp;
	bool end;

	int SDLflags;
	bool fullScreen;
	int mDisaplayId;

	std::string title;

	int ProcessSDLEvents();
public:

	int Initialize(std::string title_, int width_, int height_, bool fullScreen_);
	int MainLoop();

	Application();
	virtual ~Application();
private:
	int init(int display_index = 0);
	void free();


};

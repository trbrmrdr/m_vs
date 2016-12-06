#ifndef _APPLICATION_H
#define _APPLICATION_H
#include "stdafx.h"
#include <SDL.h>
#include <SDL_video.h>

namespace App {

	class Application {
	private:
		int widthMax;
		int heightMax;

		int width;
		int height;

		int currWidth;
		int currHeight;


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

};

#endif

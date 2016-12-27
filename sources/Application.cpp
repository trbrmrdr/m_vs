#include "stdafx.h"
#include "Application.h"
#include "Core.h"

bool one = true;

Application::Application():
	end(false),
	maxSize(-1.),
	fullScreen(false),
	mSDL_Window(NULL) {

	// OpenAL
	//audioAnalyzer = new AudioAnalyzer(44100, 1024);

}

Application::~Application() {
	free();
	SDL_Quit();
}

int Application::Initialize(std::string title_, int width, int height, bool fullScreen_) {
	fullScreen = fullScreen_;
	title = title_;
	startSize = Size(width, height);
	return init();
}

int Application::init() {

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOGGER()->OutputString("Error: " + std::string(SDL_GetError()));
		return -1;
	}
	LOGGER()->OutputString("SDL_Init succeeded");

	//const SDL_VideoInfo* info = NULL;
	//SDL_WM_SetCaption(, NULL);

	//SDL_DisplayMode mode;

	SDL_DisplayMode mode = {SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0};
	int display_count = 0;// SDL_GetNumVideoDisplays();
	int display_index = 0;
	int mode_index = 0;
	if (SDL_GetDisplayMode(display_index, mode_index, &mode) != 0)
	{
		SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
		return 1;
	}

	//info = SDL_GetVideoDriver();
	if (-1 == maxSize.width)
	{
		maxSize = Size(mode.w, mode.h);
	}
	if (fullScreen)
	{
		//SDLflags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | ;
		SDLflags = SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_SKIP_TASKBAR;
		currSize = maxSize;
		//SDL_EnableScreenSaver();
	}
	else
	{
		SDLflags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
		currSize = startSize;
	}
	//SDL_CaptureMouse(fullScreen ? SDL_TRUE : SDL_FALSE);
	LOGGER()->OutputString("SDL_GetVideoInfo succeeded");

	//bpp = info->vfmt->BitsPerPixel;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if (NULL != mSDL_Window)
		SDL_DestroyWindow(mSDL_Window);

	mSDL_Window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, currSize.width, currSize.height, SDLflags);
	if (NULL == mSDL_Window)
	{
		LOGGER()->OutputString("Error: " + std::string(SDL_GetError()));
		return -1;
	}
	SDL_GLContext glcontext = SDL_GL_CreateContext(mSDL_Window);
	if (NULL == mSDL_Window)
	{	// если не получилось создать окно, то выходим
		exit(1);
	}


	LOGGER()->OutputString("SDL_SetVideoMode succeeded");

	if (glewInit() != GLEW_OK)
	{
		LOGGER()->OutputString("Error: glewInit()");
		return -1;
	}
	LOGGER()->OutputString("glewInit succeeded");

	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	//SDL_EnableUNICODE(true);
	//SDL_EnableKeyRepeat(250, 25);
	//SDL_WM_GrabInput(SDL_GRAB_FULLSCREEN);

	int ret = CORE().init(currSize);
	return ret;
}

void Application::free() {
	CORE().free();
}

int mFullScreen = 0;

int	Application::ProcessSDLEvents() {
	SDL_Event eve;
	// Process mouse events
	SDL_PumpEvents();
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	CORE().changeMouse(
		Vec2((float) mouseX / currSize.width,
			 1.0f - (float) mouseY / currSize.height));

	while (SDL_PollEvent(&eve))
	{
		switch (eve.type)
		{
			case SDL_WINDOWEVENT_RESIZED:
				CORE().free();
				currSize = Size(eve.window.data1, eve.window.data2);
				CORE().init(currSize);
				return 1;
			case SDL_KEYDOWN:
				{
					if (eve.key.keysym.sym == SDLK_ESCAPE)
						return -1;
					//Uint16 ch = keysym.scancode;
					//SDLKey key = isprint(ch)?(SDLKey)ch:keysym.sym;

					if (eve.key.keysym.mod & KMOD_CTRL && eve.key.keysym.sym == SDLK_SPACE)
					{
						static bool cursor = true;
						cursor = !cursor;
						SDL_ShowCursor(cursor);
						return 1;
					}

					if (eve.key.keysym.sym == SDLK_RETURN || eve.key.keysym.sym == SDLK_LALT)
					{
						mFullScreen++;
					}

					if (mFullScreen == 2)
					{
						mFullScreen = 0;
						fullScreen = !fullScreen;
						free();
						init();
						return 1;
					}
					int ret = CORE().changeKeys(eve.key.keysym);
					if (ret != 0)
						return ret;

				}
				break;
			case SDL_KEYUP:
				if (eve.key.keysym.sym == SDLK_RETURN || eve.key.keysym.sym == SDLK_LALT)
				{
					mFullScreen = MAX(0, mFullScreen - 1);
				}

				break;

			case SDL_QUIT:
				return -1;
		}

	}

	return 0;
}

int Application::MainLoop() {
	LOGGER()->OutputString("MainLoop...");
	while (!end)
	{
		if (ProcessSDLEvents() < 0)
			break;

		CORE().update(SDL_GetTicks());

		SDL_GL_SwapWindow(mSDL_Window);
	}

	LOGGER()->OutputString("End");

	return 0;
}
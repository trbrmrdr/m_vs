#include "Application.h"
#include "Scene1.h"
bool one = true;

using namespace App;

Scene1 mScene1;

Application::Application():
	width(0), height(0),
	end(false),
	widthMax(-1),
	heightMax(-1),
	fullScreen(false),
	SDLflags(SDL_OPENGL) {

	// OpenAL
	//audioAnalyzer = new AudioAnalyzer(44100, 1024);

}

Application::~Application() {
	free();
}


int Application::Initialize(std::string title_ , int width_, int height_, bool fullScreen_) {
	fullScreen = fullScreen_;
	title = title_;

	width = width_;
	height = height_;
	return init();
}

int Application::init() {
	const SDL_VideoInfo* info = NULL;

	if (SDL_Init(SDL_INIT_VIDEO)<0)
	{
		LOGGER()->OutputString("Error: "+std::string(SDL_GetError()));
		return -1;
	}
	LOGGER()->OutputString("SDL_Init succeeded");


	SDL_WM_SetCaption(title.c_str(), NULL);

	info = SDL_GetVideoInfo();
	if (-1==widthMax)
	{
		widthMax = info->current_w;
		heightMax = info->current_h;
	}
	if (fullScreen)
	{
		SDLflags = SDL_OPENGL|SDL_FULLSCREEN;
		currWidth = widthMax;
		currHeight = heightMax;
	}
	else
	{
		SDLflags = SDL_OPENGL|SDL_RESIZABLE;
		currWidth = width;
		currHeight = height;
	}


	if (!info)
	{
		LOGGER()->OutputString("Error: "+std::string(SDL_GetError()));
		return -1;
	}
	LOGGER()->OutputString("SDL_GetVideoInfo succeeded");

	bpp = info->vfmt->BitsPerPixel;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	mSDL_Surface = SDL_SetVideoMode(currWidth, currHeight, bpp, SDLflags);
	if (NULL==mSDL_Surface)
	{
		LOGGER()->OutputString("Error: "+std::string(SDL_GetError()));
		return -1;
	}
	LOGGER()->OutputString("SDL_SetVideoMode succeeded");

	if (glewInit()!=GLEW_OK)
	{
		LOGGER()->OutputString("Error: glewInit()");
		return -1;
	}
	LOGGER()->OutputString("glewInit succeeded");

	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	SDL_EnableUNICODE(true);
	SDL_EnableKeyRepeat(250, 25);

	
	int ret = mScene1.init(currWidth, currHeight);
	return ret;
}

void Application::free() {
	mScene1.free();
}

int	Application::ProcessSDLEvents() {
	SDL_Event eve;
	// Process mouse events
	SDL_PumpEvents();
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	mScene1.changeMouse(mouseX, mouseY);

	while (SDL_PollEvent(&eve))
	{
		switch (eve.type)
		{
			case SDL_KEYDOWN:
				{
					if (eve.key.keysym.sym==SDLK_ESCAPE)
						return -1;

					if (eve.key.keysym.mod & KMOD_CTRL && eve.key.keysym.sym==SDLK_SPACE)
					{
						static bool cursor = true;
						cursor = !cursor;
						SDL_ShowCursor(cursor);
						return 1;
					}

					if (eve.key.keysym.sym==SDLK_RETURN && eve.key.keysym.mod & SDLK_LAST)
					{
						fullScreen = !fullScreen;
						free();
						init();
						return 1;
					}
					int ret = mScene1.changeKeys(eve.key.keysym);
					if (ret!=0)
						return ret;

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
		if (ProcessSDLEvents()<0)
			break;

		mScene1.Update(SDL_GetTicks());

		SDL_GL_SwapBuffers();
	}

	LOGGER()->OutputString("End");

	return 0;
}
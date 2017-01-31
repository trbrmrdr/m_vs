#include "stdafx.h"
#include "Application.h"
#include "Core.h"

bool one = true;

Application::Application() :
	end(false),
	fullScreen(false),
	mSDL_Window(NULL)
{

	// OpenAL
	//audioAnalyzer = new AudioAnalyzer(44100, 1024);

}

Application::~Application()
{
	free();
	SDL_Quit();
}

int Application::Initialize(std::string title_, int width, int height, bool fullScreen_)
{
	fullScreen = fullScreen_;
	title = title_;
	startSize = Size(width, height);
	mDisaplayId = 0;
	return init(mDisaplayId);
}

int Application::init(int display_index)
{

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOGGER()->OutputString("Error: " + std::string(SDL_GetError()));
		return -1;
	}
	LOGGER()->OutputString("SDL_Init succeeded");

	//const SDL_VideoInfo* info = NULL;
	//SDL_WM_SetCaption(, NULL);

	//SDL_DisplayMode mode;
	if (maxSizeDisplay.empty())
	{
		for (uint di = 0; di < SDL_GetNumVideoDisplays(); ++di)
		{
			SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };
			int mode_index = 0;
			//if (SDL_GetDisplayMode(di, mode_index, &mode) != 0)
			if (SDL_GetCurrentDisplayMode(di, &mode) != 0)
			{
				LOGF("SDL_GetDisplayMode failed: %s", SDL_GetError());
				return 1;
			}
			maxSizeDisplay.push_back(Size(mode.w, mode.h));
		}
	}

	if (fullScreen)
	{
		//SDLflags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | ;
		SDLflags = SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_SKIP_TASKBAR;
		currSize = maxSizeDisplay[display_index];
	}
	else
	{
		SDLflags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
		currSize = startSize;
	}
	//SDL_CaptureMouse(fullScreen ? SDL_TRUE : SDL_FALSE);
	LOGGER()->OutputString("SDL_GetVideoInfo succeeded");

	//bpp = info->vfmt->BitsPerPixel;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

	if (NULL != mSDL_Window)
		SDL_DestroyWindow(mSDL_Window);

	mSDL_Window = SDL_CreateWindow(title.c_str(),
								   SDL_WINDOWPOS_CENTERED_DISPLAY(display_index), SDL_WINDOWPOS_CENTERED_DISPLAY(display_index),
								   currSize.width, currSize.height,
								   SDLflags);
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
	//mSDL_Window->

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

void Application::free()
{
	CORE().free();
}

int mFullScreen = 0;

int	Application::ProcessSDLEvents()
{
	SDL_Event eve;
	// Process mouse events
	SDL_PumpEvents();
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	CORE().changeMouse(Vec2((float) mouseX / currSize.width, 1.0f - (float) mouseY / currSize.height));

	while (SDL_PollEvent(&eve))
	{
		int kay_press = -1;
		switch (eve.type)
		{
			case SDL_WINDOWEVENT_RESIZED:
				CORE().free();
				currSize = Size(eve.window.data1, eve.window.data2);
				CORE().init(currSize);
				return 1;
			case SDL_KEYDOWN:
				kay_press = 0;
				break;
			case SDL_KEYUP:
				kay_press = 1;
				break;
			case SDL_QUIT:
				return -1;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				Uint32 mouseState = SDL_GetMouseState(NULL, NULL);
				bool left_click = mouseState & SDL_BUTTON(SDL_BUTTON_LEFT);
				bool right_click = mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT);
				CORE().changeMouseKeys(left_click, right_click);
				break;
		}


		if (kay_press != -1)
		{
			const Uint8 *state = SDL_GetKeyboardState(NULL);

			if (state[SDL_SCANCODE_ESCAPE])
				return -1;
			if (state[SDL_SCANCODE_LALT])
			{
				int t = SDL_GetNumVideoDisplays();
				int newDisplayId = mDisaplayId;
				if (state[SDL_SCANCODE_1])
				{
					newDisplayId = 0;
				}
				else if (state[SDL_SCANCODE_2] && SDL_GetNumVideoDisplays() > 1)
				{
					newDisplayId = 1;
				}
				if (newDisplayId != mDisaplayId)
				{
					mDisaplayId = newDisplayId;
					if (fullScreen)
					{
						free();
						init(mDisaplayId);
					}
					else
					{
						SDL_SetWindowPosition(mSDL_Window, SDL_WINDOWPOS_CENTERED_DISPLAY(mDisaplayId), SDL_WINDOWPOS_CENTERED_DISPLAY(mDisaplayId));
					}
				}
			}
			if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_SPACE])
			{
				static bool cursor = true;
				cursor = !cursor;
				SDL_ShowCursor(cursor);
				return 1;
			}

			if (state[SDL_SCANCODE_RETURN] &&
				(state[SDL_SCANCODE_LALT] || state[SDL_SCANCODE_RALT]))
			{
				fullScreen = !fullScreen;
				free();
				init(mDisaplayId);
				return 1;
			}

			int ret = CORE().changeKeys(state, kay_press);
			if (ret != 0)
				return ret;
		}

	}

	return 0;
}

int Application::MainLoop()
{
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
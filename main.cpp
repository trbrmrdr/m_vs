#include "stdafx.h"
#include "Application.h"
#include "Core.h"

FILE _iob[] = {*stdin, *stdout, *stderr};

extern "C" FILE * __cdecl __iob_func(void) {
	return _iob;
}


#ifdef _DEBUG
#include <crtdbg.h>
//#define _CRTDBG_MAP_ALLOC
#endif

int main(int argc, char** argv) {
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	//_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	//_CrtDumpMemoryLeaks();

#if 1
	App::Application application;

	if (application.Initialize("visual app", 1280, 720, false)<0)
	{
		return -1;
	}
	application.MainLoop();
#else
	LiveCoder::Core core;
	int fullScreent = false;
	int width = 1280;
	int height = 720;

	if (argc>=2)
	{
		if (strcmp(argv[1], "-f")==0)
		{
			fullScreent = true;
		}
	}
	if (argc>=4)
	{
		width = atoi(argv[2]);
		height = atoi(argv[3]);
	}

	if (core.Initialize("Live Coder", width, height, fullScreent)<0)
	{
		return -1;
	}
	core.MainLoop();
#endif
	return 0;
}

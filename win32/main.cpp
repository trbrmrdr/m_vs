#include "stdafx.h"
#include "Application.h"
#include "Core.h"

/*
FILE _iob[] = {*stdin, *stdout, *stderr};

extern "C" FILE * __cdecl __iob_func(void) {
	return _iob;
}

#ifdef _DEBUG
#include <crtdbg.h>
//#define _CRTDBG_MAP_ALLOC
#endif
*/

int main(int argc, char** argv) {
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	//_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	//_CrtDumpMemoryLeaks();
	Application application;

	const int width = 1280;
	const int height = 720;
	if (application.Initialize("visual app", width, height, false) < 0)
	{
		return -1;
	}
	application.MainLoop();
	return 0;
}

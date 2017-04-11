#pragma once

#define TARGET_WIN32 1

typedef unsigned int uint;
typedef unsigned char uchar;

#if defined(WIN32) || defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

//#define fopen fopen_s
//#define sscanf  sscanf_s
//#define vsprintf vsprintf_s
//#define sprintf sprintf_s

#endif

#ifdef	__APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <SDL.h>
#include <SDL_video.h>
#include <FreeImage.h>



#include <string>
#include <math.h>
#include <sys/stat.h>
//#include <unistd.h>
#include <time.h>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <set>
#include <iomanip> 


#include "Logger.h"

#include "utils/Geom.h"
#include "utils/Helper.h"
#include "utils/Strings.h"
#include "utils/Math.h"
#include "utils/Singleton.h"
#include "utils/xml/xmlFile.h"



#include "KeyBuffer.h"
#include "MouseBuffer.h"
#include "ShaderGL.h"
#include "BitmapFontGL.h"
#include "TextEditor.h"
#include "KeyAnalyzer.h"
//#include "AudioAnalyzer.h"

//#include "sound_system.h"

#define SAFE_DELETE(ref) {if (NULL != ref) delete ref; ref = NULL;}

#define SET_CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define SET_CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)

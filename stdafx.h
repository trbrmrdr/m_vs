#pragma once


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

#include <sys/stat.h>
//#include <unistd.h>
#include <time.h>
#include <map>
#include <FreeImage.h>



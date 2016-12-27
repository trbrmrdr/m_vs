#pragma once
#include "stdafx.h"
using namespace std;

namespace Strings {
	//########################################################################## 

	string l_trim(string& str);
	string r_trim(string& str);
	string TrimC(const string& str);
	string Trim(string& str);

	vector<string> SplitString(const string& str, const string& aDelimiters = " ", bool bSkipEmpty = true);

	string ToUpper(const string& str);
	string ToLower(const string& str);

	int StrCmp(const string& lhs, const string& rhs);
	//int StrCmp(const wstring& lhs, const wstring& rhs);
	//########################################################################## ToStr

	template <typename T> string DoubleToString(T value, int precision = 5);
	string FloatToStr(float value, int precision = 5);
	string DoubleToStr(double value, int precision = 5);

	template <typename T > string NumberToStr(T value);
	string UnsignedToStr(unsigned int value);
	string IntToStr(int value);

	//string RectToString(const FRect& rect, const char delimiters = 0);
	string Vec2ToStr(const Vec2& point, const char delimiters = 0);

	//string ColorToStr(const Color::TColor& color, const char delimiters = 0);
	//########################################################################## StrTo

	string OnlyNumbers(const string& str);
	int StrToInt(const string& str);
	float StrToFloat(const string& str);

	signed long StrToSigned(const string& s, bool& ok);
	unsigned long StrToUnsigned(const string& str, bool& ok);
	bool StrToBool(const string&str, bool& ok);


	//Color::TColor StrToColor(const string& str, bool& ok, const char delimiters = 0);
	// empty = " " , ":" , "," 
	Vec2 StrToVec2(const string& str, const char delimiters = 0);
	//FRect StrToRect(const string& str, const char delimiters = 0);

	//##########################################################################

	inline string FormatToString(const char * format, ...) {
		string retStr("");

		if (NULL != format)
		{
			va_list marker;

			// initialize variable arguments
			va_start(marker, format);

			// Get formatted string length adding one for NULL
			size_t len = vsnprintf(NULL, 0, format, marker) + 1;
			va_end(marker);

			// Create a char vector to hold the formatted string.
			vector<char> buffer(len, '\0');

			va_start(marker, format);
			int nWritten = vsnprintf(&buffer[0], len, format, marker);

			if (nWritten > 0)
			{
				retStr = &buffer[0];
			}

			// Reset variable arguments
			va_end(marker);
		}

		return retStr;
	}

	inline string FormatToString(const char * format, va_list args) {
		// variadic args to string:
		// http://www.codeproject.com/KB/string/string_format.aspx
		char aux_buffer[10000];
		string retStr("");
		if (NULL != format)
		{

			// Get formatted string length adding one for NULL
			vsprintf(aux_buffer, format, args);
			retStr = aux_buffer;

		}
		return retStr;
	}

	//##########################################################################

	string URLEncode(const string& s);
	string URLDecode(const string& s);

};


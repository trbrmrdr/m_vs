#include "stdafx.h"
#include "Strings.h"

using namespace Strings;
using namespace std;

//########################################################################## 

string Strings::l_trim(string& str)
{
	int spos = 0;
	for (string::iterator it = str.begin(); it != str.end(); ++it)
	{
		if (*it == ' ' || *it == '\t')
			++spos;
		else
		{
			if (spos > 0)
			{
				str = str.substr(spos, -1);
			}
			break;
		}
	}
	return str;
}

string Strings::r_trim(string& str)
{
	int ecnt = 0;
	for (string::reverse_iterator it = str.rbegin(); it != str.rend(); ++it)
	{
		if (*it == ' ' || *it == '\t')
			++ecnt;
		else
		{
			if (ecnt > 0)
			{
				str = str.substr(0, str.size() - ecnt);
			}
			break;
		}
	}
	return str;
}

string Strings::TrimC(const string& str)
{
	signed long start = 0;
	signed long finish = str.size() - 1;

	while (start < (signed long) str.size() && isspace(str[start]))
		++start;
	while (finish > start && isspace(str[finish]))
		--finish;

	return str.substr(start, finish - start + 1);
}

string Strings::Trim(string& str)
{
	return r_trim(l_trim(str));
}

vector<string> Strings::SplitString(const string& str, const string& aDelimiters, bool bSkipEmpty)
{
	vector<string> ret;
	string name;
	size_t spos = 0, epos;
	do
	{
		epos = str.find_first_of(aDelimiters, spos);
		name = str.substr(spos, epos - spos);
		Strings::Trim(name);
		if (!(name.empty() && bSkipEmpty))
		{
			ret.push_back(name);
		}
		spos = epos + 1;
	}
	while (epos != string::npos);

	return ret;
}

static char toUpperFunction(char c)
{
	return toupper(c);
}

static char toLowerFunction(char c)
{
	return tolower(c);
}

string Strings::ToUpper(const string& s)
{
	string ret = s;
	transform(ret.begin(), ret.end(), ret.begin(), toUpperFunction);
	return ret;
}

string Strings::ToLower(const string& s)
{
	string ret = s;
	transform(ret.begin(), ret.end(), ret.begin(), toLowerFunction);
	return ret;
}

int Strings::StrCmp(const string& lhs, const string& rhs)
{
#if defined(_WIN32) || defined(WIN32)
	return _stricmp(lhs.c_str(), rhs.c_str());
#elif defined (POSIX)
	return strcasecmp(lhs.c_str(), rhs.c_str());
#else
	return strcasecmp(lhs.c_str(), rhs.c_str());
#endif
}

/*
int Strings::StrCmp(const wstring& lhs, const wstring& rhs) {
#if defined(WIN32)
	return _wcsicmp(lhs.c_str(), rhs.c_str());
#elif defined (POSIX)
	return wcscasecmp(lhs.c_str(), rhs.c_str());
#else
	return 0;
#endif
}
*/
//########################################################################## 

template <class T> string Strings::DoubleToString(T value, int precision)
{
	stringstream sstr;
	sstr << fixed << setprecision(precision) << value;
	return sstr.str();
}

string Strings::FloatToStr(float value, int precision)
{
	return DoubleToString(value, precision);
}

string Strings::DoubleToStr(double value, int precision)
{
	return DoubleToString(value, precision);
}

template <class T >
string Strings::NumberToStr(T value)
{
	stringstream sstr;
	sstr << value;
	return sstr.str();
}

string Strings::UnsignedToStr(unsigned int value)
{
	return NumberToStr(value);
}

string Strings::IntToStr(int value)
{
	return NumberToStr(value);
}

/*
string RectToString(const FRect& rect, const char delimiters) {
	string ret;
	string del = " ";
	if (delimiters != 0) del[0] = delimiters;
	ret.append(
		FloatToStr(rect.pos.x) + del +
		FloatToStr(rect.pos.y) + del +
		FloatToStr(rect.width) + del +
		FloatToStr(rect.height)
	);
	return ret;
}

string ColorToStr(const TColor& color, const char delimiters) {
	string ret;
	if (delimiters == 0)
	{
		ret = UnsignedToStr(color.getColor_t());
	}
	else
	{
		string del = " ";
		del[0] = delimiters;
		ret.append(
			IntToStr((int) ( 255.f * color.r )) + del +
			IntToStr((int) ( 255.f * color.g )) + del +
			IntToStr((int) ( 255.f * color.b )) + del +
			IntToStr((int) ( 255.f * color.a ))
		);
	}
	return ret;
}
*/


string Strings::Vec2ToStr(const Vec2& point, const char delimiters)
{
	string ret;
	string del = " ";
	if (delimiters != 0) del[0] = delimiters;
	ret.append(
		FloatToStr(point.y) + del +
		FloatToStr(point.x)
		);
	return ret;
}


//########################################################################## 

string Strings::OnlyNumbers(const string& str)
{
	string ret;
	for (uint i = 0, i2 = 0, i3 = 0; i < str.length(); ++i)
	{
		const char ch = str[i];
		if (ch >= '0' && ch <= '9')
		{
			ret += ch;
		}
		else if ((ch == '+' || ch == '-') && i2 == 0)
		{
			i2++;
			ret += ch;
		}
		else if (ch == '.' && i3 == 0)
		{
			i3++;
			ret += ch;
		}
	}
	return ret;
}

int Strings::StrToInt(const string& str)
{
	int x;
	sscanf(str.c_str(), "%d", &x);
	return x;
}

float Strings::StrToFloat(const string& str)
{
	float x;
	sscanf(str.c_str(), "%f", &x);
	return x;
}

signed long Strings::StrToSigned(const string& s, bool& ok)
{
	char* end = NULL;
	signed long l = strtol(s.c_str(), &end, 10);
	ok = (end != NULL && *end == 0);
	return l;
}

unsigned long Strings::StrToUnsigned(const string& s, bool& ok)
{
	char* end = NULL;
	unsigned long l = strtoul(s.c_str(), &end, 10);
	return l;
}

bool Strings::StrToBool(const string&s, bool& ok)
{

	char* end = NULL;
	unsigned long l = strtoul(s.c_str(), &end, 10);
	ok = (end != NULL && *end == 0);
	bool ret = l != 0;
	if (!ok)
	{
		string tmp = Strings::ToLower(s);
		if (tmp.compare("true") == 0)
		{
			ret = true;
			ok = true;
		}
		else if (tmp.compare("false") == 0)
		{
			ret = false;
			ok = true;
		}
	}
	return ret;
}

/*
TColor StrToColor(const string& str, bool& ok, const char delimiters) {
	TColor ret = None;
	if (delimiters != 0)
	{
		int k1 = 0, k2 = 0;
		uint i = 0;
		for (; i <= 3; ++i)
		{
			k2 = str.find(delimiters);
			if (k2 == string::npos)break;
			float res = (float) ( StrToInt(str.substr(k1, k2 + 1)) / 255.f );
			switch (i)
			{
				case 0: ret.r = res; break;
				case 1: ret.g = res; break;
				case 2: ret.b = res; break;
				case 3: ret.a = res; break;
			}
			k1 = k2 + 1;
		}
		if (i != 4)
			ret = None;
	}

	if (ret == None)
	{
		unsigned long l = StrToUnsigned(str, ok);
		ret = TColor(l);
	}
	return ret;
}


FRect StrToRect(const string& str, const char delimetrs) {
	FRect ret;
	int k1 = 0, k2 = 0;
	char del = delimetrs;
	if (del == 0)
		del = ' ';
	k2 = str.find(del);
	ret.pos.x = StrToFloat(str.substr(k1, k2 + 1));
	k1 = k2 + 1;
	k2 = str.find(del, k1);
	ret.pos.y = StrToFloat(str.substr(k1, k2 + 1));
	k1 = k2 + 1;
	k2 = str.find(del, k1);
	ret.width = StrToFloat(str.substr(k1, k2 + 1));
	k1 = k2 + 1;
	ret.height = StrToFloat(str.substr(k1));
	return ret;
}
/**/

Vec2 Strings::StrToVec2(const string& str, const char delimiters)
{
	Vec2 point;
	//sscanf(pointString.c_str(), "%f %f %f", &point.x, &point.y, &point.z);
	if (delimiters == 0)
		sscanf(str.c_str(), "%f %f", &point.x, &point.y);
	else
	{
		int k = str.find(delimiters);
		point.x = Strings::StrToFloat(str.substr(0, k));
		point.y = Strings::StrToFloat(str.substr(k + 1));
	}
	return point;
}


//###########################################################################


string Strings::URLEncode(const string& s)
{
	string ret;
	char buf[16];
	for (string::const_iterator cit = s.begin(); cit != s.end(); ++cit)
	{
		unsigned char c = *cit;
		if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '-' || c == '.' || c == '_' || c == '~')
			ret += c;
		else
		{
			sprintf(buf, "%%%02X", c);
			ret += buf;
		}
	}
	return ret;
}

string Strings::URLDecode(const string& s)
{
	string ret;
	for (string::const_iterator cit = s.begin(); cit != s.end(); ++cit)
	{
		if (*cit == '%')
		{
			char ch[3] = { 0, 0, 0 };
			++cit;
			if (cit == s.end())
				break;
			ch[0] = *cit;
			++cit;
			if (cit == s.end())
				break;
			ch[1] = *cit;
			char* end;
			char c = (char) strtoul(ch, &end, 16);
			if (end == ch + 2)
				ret += c;
		}
		else
		{
			ret += *cit;
		}
	}
	return ret;
}


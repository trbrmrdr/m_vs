#pragma once
#include <stdafx.h>
#define ABS(x) ((x)>0?(x):-(x))
#define SIGN(x) ((x)>0?+1:(x)<0?-1:0)

#define MAX(x,y) ((x)>=(y)?(x):(y))
#define MIN(x,y) ((x)<=(y)?(x):(y))

namespace Math {

	void SeedRandom();
	template <typename T>T Random();

	template <typename T>T Random(const T& min, const T& max);

	const float PI = 3.1415926535897932384626433832795f;
	const float PI_2 = 3.1415926535897932384626433832795f / 2.0f;
	const float PI_3 = 3.1415926535897932384626433832795f / 3.0f;
	const float PI_4 = 3.1415926535897932384626433832795f / 4.0f;
	const float PI_6 = 3.1415926535897932384626433832795f / 6.0f;
	const float FloatMaxValue = FLT_MAX;

	const float TO_DEGREES = 180.f / 3.1415926535897932384626433832795f;
	const float TO_RADIANS = 3.1415926535897932384626433832795f / 180.f;

	const float DEG_15 = 0.26179938779914943653855361527329f;
	const float DEG_30 = 0.52359877559829887307710723054658f;
	const float DEG_45 = 0.78539816339744830961566084581988f;
	const float DEG_60 = 1.0471975511965977461542144610932f;
	const float DEG_90 = 1.5707963267948966192313216916398f;

	int Random();

	int Random(int min, int max);

	inline double Random(double min, double max);

	inline float Random(float min, float max);

	inline int RandomSign();

	template <class T> inline T NormalizeAngle(const T& angle);

	template <class T> inline T ToRadians(const T& deg);

	template <class T> inline T ToDegrees(const T& rad);

	template <class T> inline T Lerp(const T& a, const T& b, float t);

	template <class T> inline T Serp(const T& a, const T& b, float t);

	template <class T> inline T Lerp(const T& t);

	template <class T> inline T Serp(const T& t);

	template<typename T> inline  T Clamp(const T& value, const T& min, const T& max);

	 Vec2 Clamp(const Vec2& value, const Vec2& min, const Vec2& max);
	inline bool floatsEqual(float a, float b, float epsilon = 0.00001f);

};

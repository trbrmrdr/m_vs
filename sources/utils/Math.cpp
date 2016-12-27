#include "stdafx.h"
#include "Math.h"

using namespace std;
using namespace Math;

static unsigned int random_init = 0;

void Math::SeedRandom() {
	random_init = 1;
#if defined(WIN32) || defined(_WIN32)
	srand((uint) time(NULL));
#else
	struct timeval tv;
	gettimeofday(&tv, 0);
	long int n = ( tv.tv_sec ^ tv.tv_usec ) ^ getpid();
	srand(n);
#endif
}

template <typename T>
T Math::Random() {
	if (random_init == 0)
	{
		SeedRandom();
	}
	return ( (T) std::rand() / RAND_MAX );
}

template <typename T>
T Math::Random(const T& min, const T& max) {
	if (max == min)
		return min;
	T _max = MAX(min, max);
	T _min = MIN(min, max);

	return (T) ( _min + ( _max - _min ) * Random<float>() );
}

int Math::Random() {
	return Random<int>();
}

int Math::Random(int min, int max) {
	return  Random<int>(min, max);
}

double Math::Random(double min, double max) {
	return  Random<double>(min, max);
}

float Math::Random(float min, float max) {
	return Random<float>(min, max);
}


inline int Math::RandomSign() {
	return Random(-1, +1);
}

template <class T>
T Math::NormalizeAngle(const T& angle) {
	while (angle >= PI * 2)
		angle -= PI * 2;
	while (angle < 0)
		angle += PI * 2;
	return angle;
}

template <class T>
T Math::ToRadians(const T& deg) {
	return deg * TO_RADIANS;
}

template <class T>
T Math::ToDegrees(const T& rad) {
	return rad * TO_DEGREES;
}

template <class T>
T Math::Lerp(const T& a, const T& b, float t) {
	return (T) ( a + t * ( b - a ) );
}

template <class T>
T Math::Serp(const T& a, const T& b, float t) {
	if (t == 1)
		return b;
	else if (t == 0)
		return a;
	return a + sin(t * PI / 2) * ( b - a );
}

template <class T>
T Math::Lerp(const T& t) {
	return Lerp((T) 0, (T) 1, t);
}

template <class T>
T Math::Serp(const T& t) {
	return Serp(T(0), T(1), t);
}

template<typename T>
T Math::Clamp(const T& value,const T& min,const T& max) {
	return ( value < min ) ? min : ( value > max ) ? max : value;
}

//_______________________________________________________________________________________________
bool Math::floatsEqual(float a, float b, float epsilon) {
	return ABS(a - b) < epsilon;
}



Vec2  Math::Clamp(const Vec2& value, const Vec2& min, const Vec2& max) {
	Vec2 ret = value;
	ret.x = Clamp<float>(ret.x, min.x, max.x);
	ret.y = Clamp<float>(ret.y, min.y, max.y);
	return ret;
}


#pragma once
#include "stdafx.h"




class Vec2 {
public:
	float x;
	float y;
	Vec2():x(.0), y(.0) {}
	Vec2(float x, float y):x(x), y(y) {}
	Vec2(float a):x(a), y(a) {}
	~Vec2() {}
};

#pragma once
#include "stdafx.h"

struct Size;

struct Vec2
{
	float x;
	float y;
	Vec2() :x(.0), y(.0) {}
	Vec2(float x, float y) :x(x), y(y) {}
	Vec2(float a) :x(a), y(a) {}
	~Vec2() {}

	inline Vec2 operator +(const Vec2& vec2) { return Vec2(x + vec2.x, y + vec2.y); }

	Vec2& operator=(const Vec2& vec2)
	{
		x = vec2.x;
		y = vec2.y;
		return *this;
	}
};


struct Vec3
{
	float x;
	float y;
	float z;
	Vec3() :x(.0), y(.0), z(.0) {}
	Vec3(const Vec2& vec2, float z) : x(vec2.x), y(vec2.y), z(z) {}
	Vec3(float x, float y, float z) :x(x), y(y), z(z) {}
	Vec3(float a) :x(a), y(a), z(a) {}
	~Vec3() {}

	inline Vec3 operator +(const Vec3& vec3) { return Vec3(x + vec3.x, y + vec3.y, z + vec3.z); }

	Vec3& operator=(const Vec3& vec3)
	{
		x = vec3.x;
		y = vec3.y;
		z = vec3.z;
		return *this;
	}
};


struct Size
{
	float width;
	float height;
	Size() :width(.0), height(.0) {}
	Size(float width, float height) :width(width), height(height) {}
	Size(int width, int height) :width(width), height(height) {}

	Size(float size) :width(size), height(size) {}
	~Size() {}

	inline operator Vec2() const { return Vec2(width, height); }

	Size& operator=(const Size& size)
	{
		width = size.width;
		height = size.height;
		return *this;
	}

	Size& operator=(const Vec2& vec2)
	{
		width = vec2.x;
		height = vec2.y;
		return *this;
	}
};

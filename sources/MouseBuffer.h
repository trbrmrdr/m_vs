#pragma once 
#include "stdafx.h"

#define MOUSE() (MouseBuffer::instance())

class MouseBuffer : public Singleton<MouseBuffer>
{
private:
	Vec2 pos;
	bool left = false;
	bool right = false;
public:

	bool LeftIsPress() { return left; }
	void SetKeys(bool _left, bool _right) { left = _left; right = _right; }
	void SetPosition(float cursorX, float cursorY) { pos.x = cursorX; pos.y = cursorY; }

	void SetPosition(const Vec2& _pos) { pos = _pos; }

	float GetCursorX() { return pos.x; }
	float GetCursorY() { return pos.y; }
	const Vec2& getCursorPos() { return pos; }

	MouseBuffer(void) : pos() {}
	~MouseBuffer(void) {}
};

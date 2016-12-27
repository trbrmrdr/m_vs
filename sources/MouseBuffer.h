#pragma once 
#include "stdafx.h"

#define MOUSE() (MouseBuffer::instance())

class MouseBuffer: public Singleton<MouseBuffer> {
private:
	Vec2 pos;

public:
	void SetPosition(float cursorX, float cursorY) {
		pos.x = cursorX;
		pos.y = cursorY;
	}

	void SetPosition(const Vec2& _pos) { pos = _pos; }

	float GetCursorX() { return pos.x; }
	float GetCursorY() { return pos.y; }
	Vec2 getCursorPos() { return pos; }

	MouseBuffer(void): pos() {}
	~MouseBuffer(void) {}
};

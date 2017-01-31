#pragma once
#include "stdafx.h"
#include "TextEditor.h"


class KeyAnalyzer {
private:
public:
	void KeyHit(TextEditor* textEditor, SDL_Keysym keysym, const std::string& filename);


	KeyAnalyzer(void);
	~KeyAnalyzer(void);
};
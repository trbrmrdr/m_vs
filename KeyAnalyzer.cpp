#include "KeyAnalyzer.h"

using namespace App;

bool IsMoveKey(SDL_Keycode keyKode) {
	if (keyKode == SDLK_UP || keyKode == SDLK_DOWN || keyKode == SDLK_LEFT || keyKode == SDLK_RIGHT ||
		keyKode == SDLK_HOME || keyKode == SDLK_END
		 || keyKode == 'p' || keyKode == 'P' || keyKode == 'n' || keyKode == 'N' || keyKode == 'b' || keyKode == 'B' || keyKode == 'f' || keyKode == 'F'
		)
	{
		return true;
	}
	return false;
}

void KeyAnalyzer::KeyHit(TextEditor* textEditor, SDL_Keysym keysym, const std::string& filename) {
	/*
	if ((mod & KMOD_SHIFT) && IsMoveKey(keyKode)) {
		if (!textEditor->IsSelectMode())
			textEditor->BeginSelect();
	} else {
		textEditor->EndSelect();
	}*/

	if (IsMoveKey(keysym.sym))
	{
		if (keysym.mod & KMOD_SHIFT)
		{
			if (!textEditor->IsSelectMode())
				textEditor->BeginSelect();
		}
		else
		{
			textEditor->EndSelect();
		}
	}

	int keyKode = keysym.sym;// SDL_GetKeyName(keysym.sym)[0];
	if (keysym.mod & KMOD_CTRL)
	{
		// unix like keybind
		if (keyKode == 'p' || keyKode == 'P')
			textEditor->MoveCursor(ECMT_UP);
		else if (keyKode == 'n' || keyKode == 'N')
			textEditor->MoveCursor(ECMT_DOWN);
		else if (keyKode == 'b' || keyKode == 'B')
			textEditor->MoveCursor(ECMT_LEFT);
		else if (keyKode == 'f' || keyKode == 'F')
			textEditor->MoveCursor(ECMT_RIGHT);

		// shortcut keyKode
		else if (keyKode == 'z' || keyKode == 'Z')
			textEditor->Undo();
		else if (keyKode == 'y' || keyKode == 'Y')
			textEditor->Redo();
		else if (keyKode == 's' || keyKode == 'S')
			textEditor->Save(filename);
		else if (keyKode == 'o' || keyKode == 'O')
			textEditor->Load(filename);
		else if (keyKode == 'a' || keyKode == 'A')
			textEditor->Home();
		else if (keyKode == 'e' || keyKode == 'E')
			textEditor->End();
		else if (keyKode == 'c' || keyKode == 'C')
			textEditor->Copy();
		else if (keyKode == 'x' || keyKode == 'X')
		{
			textEditor->Copy();
			textEditor->DeleteSelectedArea();
		}
		else if (keyKode == 'v' || keyKode == 'V')
			textEditor->Paste();
		else if (keyKode == SDLK_HOME)
			textEditor->MoveHead();
		else if (keyKode == SDLK_END)
			textEditor->MoveTail();
	}
	else if (SDLK_TAB <= keyKode && keyKode <= SDLK_z)
	{
		if (textEditor->IsSelectMode())
		{
			textEditor->DeleteSelectedArea();
		}
		textEditor->InsertCharacter(keyKode);
	}
	else
	{
		switch (keyKode)
		{
			case SDLK_UP:
				textEditor->MoveCursor(ECMT_UP);
				break;
			case SDLK_DOWN:
				textEditor->MoveCursor(ECMT_DOWN);
				break;
			case SDLK_LEFT:
				textEditor->MoveCursor(ECMT_LEFT);
				break;
			case SDLK_RIGHT:
				textEditor->MoveCursor(ECMT_RIGHT);
				break;

			case SDLK_PAGEUP:
				textEditor->PageUp();
				break;
			case SDLK_PAGEDOWN:
				textEditor->PageDown();
				break;

			case SDLK_HOME:
				textEditor->Home();
				break;
			case SDLK_END:
				textEditor->End();
				break;
			case SDLK_RETURN:
				textEditor->InsertCharacter('\n');
				break;
			case SDLK_BACKSPACE:
				if (textEditor->IsSelectMode())
				{
					textEditor->DeleteSelectedArea();
				}
				else
				{
					textEditor->Backspace();
				}
				break;
			case SDLK_DELETE:
				if (textEditor->IsSelectMode())
				{
					textEditor->DeleteSelectedArea();
				}
				else
				{
					textEditor->Delete();
				}
				break;
			case SDLK_TAB:
				textEditor->InsertCharacter(' ');
				textEditor->InsertCharacter(' ');
				break;
		}
	}
}


KeyAnalyzer::KeyAnalyzer(void) {}


KeyAnalyzer::~KeyAnalyzer(void) {}

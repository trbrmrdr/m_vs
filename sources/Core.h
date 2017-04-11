#pragma once
#include "stdafx.h"
#include "Scenes/Scenes.h"

#define CORE() (Core::instance())

class Core: public Singleton<Core> {
private:
	Scenes m_scenes;

	Size m_windowSize;

	bool m_editMode;
	bool m_nowCompiled;

	//KeyBuffer keyBuffer;
	//	ShaderGL postEffect;
	//TextEditor textEditor;
	//KeyAnalyzer keyAnalyzer;

	//sound_system_c sound_system;

	std::string m_nowSource;

	Uint32 m_baseTime;
	Uint32 m_prevTime;

	uint m_frame;
	uint m_fps;

	float* m_audioSpectrL = NULL;
	float* m_audioSpectrR = NULL;
	GLuint m_audioTexture;

public:

	const Size& getWindowSize(uint window = 0) { return m_windowSize; }
	int init(const Size& windows);
	void free();

	int update(Uint32 nowTime);
	void render(float realSec);

	void changeMouseKeys(bool left, bool right) { MOUSE().SetKeys(left, right); }

	void changeMouse(const Vec2& pos) { MOUSE().SetPosition(pos); }


	int changeKeys(const Uint8 *state, bool press);

	void loadEffectCallback(uint effectId);

	Core();
	~Core() { free(); }
};

#pragma once
#include "stdafx.h"
#include "Scenes.h"

#define CORE() (Core::instance())

class Core: public EffectLoadCallback, public Singleton<Core> {
private:
	Scenes scenes;
	int FPS;

	Size windowSize;


	bool editMode;
	bool nowCompiled;
	bool errorHighlight;


	KeyBuffer keyBuffer;
	//	ShaderGL postEffect;
	TextEditor textEditor;
	KeyAnalyzer keyAnalyzer;

	sound_system_c sound_system;

	std::string nowSource;

	Uint32 baseTime;
	Uint32 prevTime;
	int fps;

	float* audio_spectr_l = NULL;
	float* audio_spectr_r = NULL;
	GLuint audioTexture;

public:

	const Size& getWindowSize(uint window = 0) { return windowSize; }
	int init(const Size& windows);
	void free();

	int update(Uint32 nowTime);
	void render(float realSec);

	void changeMouseKeys(bool left, bool right);
	void changeMouse(const Vec2& pos);
	int changeKeys(const Uint8 *state,bool press);

	virtual void loadEffectCallback(uint effectId);

	Core();
	~Core() { free(); }
};

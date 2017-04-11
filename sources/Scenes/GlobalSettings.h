#pragma once
#include <stdafx.h>
#include "file.h"
#include "Scene.h"

#define GLOBAL_SETTINGS "Settings"
#define SCENE			"Scene"
#define pos_save		"posSave"
const Vec2 pos_save_def = Vec2();
#define size_save		"sizeSave"
const Size size_save_def = Size(-1.f);
#define	opt_text		"optionTexture"
const string opt_text_def = "option.png";
#define	mask_save_text	"maskSaveText"
const string mask_save_text_def = "save_text_";
#define	scene_id		"scene_id"
#define	glsl_File_name	"glsl_name"

#define iChanel			"iChannel"
#define MAX_CHANNELS		1


struct SceneSetting
{

	file file_vs;
	map<string, string> iChannels;

	SceneSetting() {}
	SceneSetting(file file_vs) :file_vs(file_vs) {}
	SceneSetting(const string& fileName_vs) :file_vs(fileName_vs) {}
};

struct GlobalSettings
{
	bool empty;
	file settingsFile;
	file optionTexture;
	string maskSaveText;
	Vec2 posSave;
	Size sizeSave;
	map<int, SceneSetting> sceneSettings;

	template<typename T>
	int newId(map<int, T> _map, int id);

	GlobalSettings(const string& fileName) :
		settingsFile(fileName),
		posSave(pos_save_def),
		sizeSave(size_save_def),
		optionTexture(opt_text_def),
		maskSaveText(mask_save_text_def)
	{}

	bool checkEdit()
	{
		if (settingsFile.checkIsEdit())
			return true;
		for (auto ss : sceneSettings)
		{
			if (ss.second.file_vs.checkIsEdit())
				return true;
		}
	}

	void save();
	void readEmptySetting();
	bool read(bool forceRead = false);
	void load(map<int, Scene*>& scenes, bool force = false);
	string getNewNameSavesText();
};
#include <stdafx.h>
#include "GlobalSettings.h"



template<typename T>
int GlobalSettings::newId(map<int, T> _map, int id)
{
	map<int, T>::iterator it = _map.find(id);
	if (it == _map.end())return id;
	int ret = _map.size();
	for (pair<int, T> it : _map)
	{
		if (it.first != ret)break;
		ret++;
	}
	return ret;
}

void GlobalSettings::save()
{
	xmlFile xml;
	xml.addTag(GLOBAL_SETTINGS);
	xml.setAttribute(GLOBAL_SETTINGS, pos_save, Strings::Vec2ToStr(posSave), 0);
	xml.setAttribute(GLOBAL_SETTINGS, size_save, Strings::Vec2ToStr(sizeSave), 0);
	xml.setAttribute(GLOBAL_SETTINGS, opt_text, optionTexture.fileName, 0);
	xml.setAttribute(GLOBAL_SETTINGS, mask_save_text, maskSaveText, 0);
	xml.pushTag(GLOBAL_SETTINGS);


	int level = 0;
	for (auto ssetting : sceneSettings)
	{
		level = xml.addTag(SCENE);
		int id = ssetting.first; id = id <= 0 ? -1 : id;
		xml.setAttribute(SCENE, scene_id, id, level);
		xml.setAttribute(SCENE, glsl_File_name, ssetting.second.file_vs.fileName, level);
		map<string, string> channels = ssetting.second.iChannels;
		for (auto it : channels)
			xml.setAttribute(SCENE, it.first, it.second, level);
	}

	xml.saveFile(settingsFile.fileName);
	settingsFile.checkIsEdit();
	empty = false;
};

void GlobalSettings::readEmptySetting()
{
	sceneSettings.clear();
	empty = true;
	posSave = pos_save_def;
	sizeSave = size_save_def;
	optionTexture = opt_text_def;
	maskSaveText = mask_save_text_def;
	vector<string> filesGlSl = Helper::GetFilesInDirectory(".", "*.glsl");
	for (auto glslfile : filesGlSl)
	{
		file tmp(glslfile);
		if (tmp.isEditData == 0)
			continue;
		sceneSettings.insert(pair<int, SceneSetting>((int) sceneSettings.size(), SceneSetting(tmp)));
	}
}

bool GlobalSettings::read(bool force)
{
	if (settingsFile.isEditData == 0)
	{
		readEmptySetting();
		return true;
	}
	if (!settingsFile.checkIsEdit() && !force)
		return false;
	xmlFile xml;
	std::string pathToFile = Helper::getDataDir() + settingsFile.fileName;
	//long tmp = Helper::GetLastDataEdit(pathToFile.c_str());
	int counter = 100;
	while (counter>0 && !xml.loadFile(pathToFile)) { counter--; }


	if (xml.getNumTags(GLOBAL_SETTINGS) == 0)
	{
		readEmptySetting();
		return true;
	}
	empty = false;
	posSave = Strings::StrToVec2(xml.getAttribute(GLOBAL_SETTINGS, pos_save, Strings::Vec2ToStr(pos_save_def), 0));
	sizeSave = Strings::StrToVec2(xml.getAttribute(GLOBAL_SETTINGS, size_save, Strings::Vec2ToStr(size_save_def), 0));
	optionTexture = xml.getAttribute(GLOBAL_SETTINGS, opt_text, opt_text_def, 0);
	maskSaveText = xml.getAttribute(GLOBAL_SETTINGS, mask_save_text, mask_save_text_def, 0);
	xml.pushTag(GLOBAL_SETTINGS);

	bool needSave = false;
	int scenes = xml.getNumTags(SCENE);
	int idNone = 0;
	sceneSettings.clear();
	for (int i = 0; i < scenes; ++i)
	{
		string glslFileName = xml.getAttribute(SCENE, glsl_File_name, "", i);
		if (glslFileName.empty())
			continue;
		string strId = xml.getAttribute(SCENE, scene_id, "", i);
		int id = 0;
		if (strId.empty())
		{
			//id = newId(glsl, 0);
			id = 0;
			needSave = true;
		}
		else
		{
			id = Strings::StrToInt(strId);
			if (id > 0)
			{
				auto it = sceneSettings.find(id);
				if (it != sceneSettings.end())
					id = 0;
				if (!needSave)
					needSave = id == 0;
				//int nid = newId(glsl, id);
				//needSave = nid != id;
				//id = nid;
			}
		}
		if (id <= 0)
			id = idNone += -1;

		map<string, string> channels;
		for (uint chi = 0; chi <= MAX_CHANNELS; chi++)
		{
			string nameChannel = Strings::FormatToString("%s%i", iChanel, chi);
			string valChannel = xml.getAttribute(SCENE, nameChannel, "", i);
			if (valChannel.empty())
				continue;
			channels.insert(pair<string, string>(nameChannel, valChannel));
		}
		SceneSetting scene(glslFileName);
		scene.iChannels = channels;
		sceneSettings.insert(pair<int, SceneSetting>(id, scene));
	}

	if (needSave)
		save();
	return true;
}

void GlobalSettings::load(map<int, Scene*>& scenes, bool force)
{
	if (force)
	{
		for (auto it : scenes)
			delete it.second;
		scenes.clear();
	}

	if (scenes.empty())
	{
		for (auto ssetting : sceneSettings)
		{
			if (ssetting.first > 0)
				scenes.insert(pair<int, Scene*>(
					ssetting.first,
					new Scene(ssetting.second.file_vs.fileName,
							  ssetting.second.iChannels))
					);
		}
		return;
	}

	for (auto scene : scenes)
	{
		file file_vs = sceneSettings[scene.first].file_vs;
		if (file_vs.checkIsEdit())
		{
			scene.second->free();
			scene.second->getSahder()->Compile();
		}
	}
}

string GlobalSettings::getNewNameSavesText()
{
	string mask = Strings::FormatToString("%s*.png", maskSaveText.c_str());
	vector<string> files = Helper::GetFilesInDirectory(Helper::getDataDir(), mask);
	int id = 0;
	int new_id = 0;
	for (vector<string>::iterator it = files.begin(), it_e = files.end(); it != it_e; ++it)
	{
		string file = Strings::ToLower(*it);
		file = file.erase(0, maskSaveText.size() + 2);//"./mask...."
		file = file.erase(file.find(".png"), 4);
		int tmp = Strings::StrToInt(file);
		if (id != new_id)
			new_id = tmp;
		else
		{
			id++;
			new_id++;
		}
	}
	do
	{
		string new_filename = Strings::FormatToString("%s%i.png", maskSaveText.c_str(), new_id);
		string path_to_file = Helper::getDataDir() + new_filename;

		if (Helper::GetLastDataEdit(path_to_file.c_str()) == 0)
			return new_filename;
		new_id++;
	}
	while (true);
	//return ;
}
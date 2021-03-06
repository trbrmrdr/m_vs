#pragma once
#include <stdafx.h>

struct file
{
	string fileName;
	long isEditData = 0;

	bool checkIsEdit()
	{
		std::string t_filePath = Helper::getDataDir() + fileName;
		long new_data = Helper::GetLastDataEdit(t_filePath.c_str());
		bool ret = new_data != isEditData && new_data != 0;
		isEditData = new_data;
		return ret;
	}

	file() {}
	file(const string& _fileName) :fileName(_fileName)
	{
		checkIsEdit();
	}

	file& operator=(const string& _fileName)
	{
		this->fileName = _fileName;
		this->checkIsEdit();
		return *this;
	}

};
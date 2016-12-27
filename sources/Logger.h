#pragma once
#include "stdafx.h"

#define LOGF(fmt,...)		Logger::Instance()->OutputString(Strings::FormatToString(fmt,__VA_ARGS__))
#define LOG(fmt)			Logger::Instance()->OutputString(fmt)

#define LOGGER Logger::Instance

class Logger {
private:
	std::vector<std::string> logs;
public:
	void PushString(std::string str) {
		logs.push_back(str);
	}
	void OutputString(std::string str) {
		std::cout << str << std::endl;
		logs.push_back(str);
	}

	std::string ToString() {
		std::string str;
		for (unsigned int i = 0; i < logs.size(); i++)
			str += logs[i] + "\n";

		return str;
	}

	void Save(void) {
		// ...
	}

	static Logger* Instance() {
		static Logger instance;
		return &instance;
	}

	Logger(const Logger& rhs) {};
	Logger& operator=(const Logger& rhs) {};

	Logger() {}

	virtual ~Logger() {}
};
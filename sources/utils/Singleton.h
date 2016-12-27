#pragma once
#include "stdafx.h"

class Noncopyable {
protected:
	Noncopyable() {}
	~Noncopyable() {}

private:
	Noncopyable(const Noncopyable&);
	const Noncopyable& operator=(const Noncopyable&);
};

template<class Ty>
class Singleton: public Noncopyable {
protected:

	static bool bIsInitialized;
	static Ty* instance_;

	Singleton() {
		bIsInitialized = true;
	};

	~Singleton() {
		bIsInitialized = false;
	};

public:

	static bool isInitialized() {
		return bIsInitialized;
	};

	static void destroyInstance() {
		if (instance_)
		{
			delete instance_;
			instance_ = NULL;
		}
	}

	static Ty& instance() {

		if (!instance_)
		{
			instance_ = new Ty;
		}

		return *instance_;
	}
};

template<class Ty> bool Singleton<Ty>::bIsInitialized = false;
template<class Ty> Ty* Singleton<Ty>::instance_ = NULL;

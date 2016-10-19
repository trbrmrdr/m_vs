#pragma once
#include "stdafx.h"

const float delAudioCalc = 3.;
const float uT_sp = 1.;

float lastSec = -1;
bool isAudioCalc = false;

struct soundVal {
	float from;
	float to;
	float lastT = 0;
	float ut = uT_sp;
	bool unic = false;
	bool isEnd = true;
	soundVal(float from, float to):from(from), to(to) {}
	soundVal(float from, float to, bool unic):from(from), to(to), unic(unic) {}

	void setVar(float newP, float sec) {
		if (!isEnd)
			return;
		if (lastT>.0)
			from = getp(sec);
		isEnd = false;
		to = newP;
		std::cout<<from<<" "<<to<<std::endl;
		lastT = sec;
	}

	float getp(float sec) {
		float retP = to;
		if (!isEnd)
		{
			float delay = sec-lastT;
			if (delay>=0.)
			{
				float compl = fmin(delay/ut, 1.);
				retP = from+(to-from) * compl;
				if (compl>=1.)
				{
					isEnd = true;
					if (unic)
					{
						float randVal = (float)rand()/(float)RAND_MAX*3.;//(0,1)
						ut = randVal;
						std::cout<<"ut = "<<randVal<<std::endl;
					}
				}
			}
		}
		return retP;
	}
};

soundVal mLow(0, 0);
soundVal mMid(0, 0);
soundVal mHigh(0, 0);


soundVal mY(0, 0, true);
soundVal mX(0, 0, true);

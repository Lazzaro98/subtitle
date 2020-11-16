#pragma once
#include<string>
class Time
{
public:
	int h, min, sec, msec;
	//primer: 01:23:42,111 
	Time(int h1 = 0, int min1 = 0, int sec1 = 0, int msec1 = 0) {
		this->h = h1;
		this->min = min1;
		this->sec = sec1;
		this->msec = msec1;
	}
	~Time() {}

	//h=1, min=23, sec=42, msec=111 => 01:23:42,111
	std::string timeToStr();
	//01:23:42,111 => h=1, min=23, sec=42, msec=111
	static Time* strToTime(std::string time);
	static bool isValid(std::string time);
};


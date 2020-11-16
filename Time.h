#pragma once
#include<string>
class Time
{
public:
	int h, min, sec, msec;
	//00:12:32,321
	Time(int h1=0, int min1=0, int sec1=0, int msec1=0) {
		this->h = h1;
		this->min = min1;
		this->sec = sec1;
		this->msec = msec1;
	}
	~Time() {}

	//h=1,min=23,.. => 01:23:42,111
	std::string timeToStr();
	static Time* strToTime(std::string time);
};


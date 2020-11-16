#include "Time.h"
std::string Time::timeToStr() {

	std::string ret = "";
	if (this->h / 10 == 0) ret += std::to_string(0);
	ret += std::to_string(h);
	ret += ":";


	if (min / 10 == 0) ret += std::to_string(0);
	ret += std::to_string(min);
	ret += ":";

	if (sec / 10 == 0) ret += std::to_string(0);
	ret += std::to_string(sec);
	ret += ",";

	if (msec / 100 == 0)ret += std::to_string(0);
	if (msec / 10 == 0)ret += std::to_string(0);
	ret += std::to_string(msec);
	return ret;

}

Time* Time::strToTime(std::string time) {
	int h, min, sec, msec;

	h = ((time[0] - '0') * 10) + (time[1] - '0');
	min = ((time[3] - '0') * 10) + (time[4] - '0');
	sec = ((time[6] - '0') * 10) + (time[7] - '0');
	msec = ((time[9] - '0') * 100) + ((time[10] - '0') * 10) + (time[11] - '0');
	Time* ret = new Time(h, min, sec, msec);
	return ret;

}

#pragma once
#include<string>
#include"Time.h"
using namespace std;
class SubtitleEditor
{
protected:
	struct Elem {
		int id;
		Time* startTime;
		Time* endTime;
		string tekst;
		Elem* next;
		Elem(int id1, Time* startTime1, Time* endTime1, string tekst1, Elem* next1=nullptr) {
			this->id = id1;
			this->startTime = startTime1;
			this->endTime = endTime1;
			this->tekst = tekst1;
			this->next = next1;
		}
	};
public:
	Elem* prvi, * posl;
	SubtitleEditor() {}
	~SubtitleEditor() {}

	int loadSubtitle(string subtitle);
	string saveSubtitle();

	void dodajNaListu(Elem* nov);
	int isNumber(char c);
};


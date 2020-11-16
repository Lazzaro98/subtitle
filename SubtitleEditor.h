#pragma once
#include<string>
#include"Time.h"
using namespace std;
class SubtitleEditor
{
private:
	string bolduj(string tekst,int start,int end);
	string iskrivi(string tekst,int start,int end);
	string podvuci(string tekst, int start, int end);
public:
	struct Elem {
		int id;
		Time* startTime;
		Time* endTime;
		string tekst;
		bool selected;
		Elem* next;
		Elem(int id1, Time* startTime1, Time* endTime1, string tekst1, Elem* next1=nullptr) {
			this->id = id1;
			this->startTime = startTime1;
			this->endTime = endTime1;
			this->tekst = tekst1;
			this->next = next1;
			this->selected = false;
		}
	};

	Elem* prvi, * posl;
	SubtitleEditor() {}
	~SubtitleEditor() {}

	int loadSubtitle(string subtitle);
	string saveSubtitle();
	void addToSelection(int start, int end);
	void deselect();
	string fontTekst(string tekst, int start, int end, char f);
	void fontOpseg(int start, int end, char f);
	void boldText(int start, int end);
	void italicText(int start, int end);
	void underlineText(int start, int end);
	void removeStyle();
	string removeStyleText(string tekst);
	void dodajNaListu(Elem* nov);
	int isNumber(char c);
};


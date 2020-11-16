#pragma once
#include<string>
#include"Time.h"
using namespace std;
class SubtitleEditor
{
private:
	//stavljamo ih u private jer su nam pomocne funkcije, koje cemo koristiti samo u ovoj klasi
	string fontTekst(string tekst, int start, int end, char f); //stilizuj jedan string, sa tag-om <f>
	void fontSelektovane(int start, int end, char f); //stilizuj sve selektovane, tagom <f>
	string removeStyleText(string tekst);//uklanja stil iz jednog(datog) string-a

	void unloadSubtitle();
	
public:
	struct Elem {
		int id;
		Time* startTime;
		Time* endTime;
		string tekst;
		bool selected;
		Elem* next;
		Elem(int id1, Time* startTime1, Time* endTime1, string tekst1, Elem* next1 = nullptr) {
			this->id = id1;
			this->startTime = startTime1;
			this->endTime = endTime1;
			this->tekst = tekst1;
			this->next = next1;
			this->selected = false;
		}
	};
	Elem* prvi, * posl;
	void dodajNaListu(Elem* nov);

	SubtitleEditor();
	~SubtitleEditor();

	int loadSubtitle(string subtitle);
	string saveSubtitle();

	void addToSelection(int start, int end);
	void deselect();

	void boldText(int start, int end); 
	void italicText(int start, int end);
	void underlineText(int start, int end);
	void removeStyle();

};


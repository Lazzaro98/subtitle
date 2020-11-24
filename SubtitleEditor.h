#pragma once
#include<string>
#include"Time.h"

using namespace std;
class SubtitleEditor
{
private:

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
	struct Elem2 {
		Elem* stanjePrvi,*stanjePosl;
		Elem2* next;
		Elem2* prev;
		Elem2(Elem* stanjePrvi1,Elem* stanjePosl, Elem2* prev, Elem2* next=nullptr) {
			this->stanjePrvi = stanjePrvi1;
			//pom = new Elem(0, stanjePosl->startTime, stanjePosl->endTime, nullptr);
			this->stanjePosl = stanjePosl;
			this->next = next;
			this->prev = prev;
		}
	};

	void unloadSubtitles();

	//stavljamo ih u private jer su nam pomocne funkcije, koje cemo koristiti samo u ovoj klasi
	string fontTekst(string tekst, int start, int end, char f); //stilizuj jedan string, sa tag-om <f> pr:Cao svima -> <b>Cao</b> svima
	void fontSelektovane(int start, int end, char f); //stilizuj sve selektovane, tagom <f>
	string removeStyleText(string tekst);//uklanja stil iz jednog(datog) string-a

	void del(Elem* pocetak, int selektId);

	//posto u zadatku pise da ukoliko bar jedan red prekoraci max
	//treba da seodradi fixLine. Zato cemo prvi funkcijom shouldFix(string,int) proveriti da li treba.
	//I nju cemo pozvati u fixLongLines1. Tako cemo znati da li fixLongLines1 treba da radi nesto,
	//ili je sve okej u tom Subtitle-u.
	bool shouldFix(string subtitle, int max);
	//vraca niz reci koje se javljaju u subtitle-u
	//uredi jedan string. Ovo se pozove za svakog clana liste kasnije
	string fixLongLines1(string subtitle, int max);
	//vraca samo da li je nesto u listi selektovano
	bool imaSelektovanih();
	string* wordsFromSubtitle(string subtitle, int* brojReci);
	Elem* podeliSubtitle(Elem* elem, int ms);
	void dodajStanje(Elem* stanjePocetni, Elem* stanjeKrajnji);
	Elem* napraviKopijuListe(Elem* prvi,Elem* last);

	void updateIDs();

	Elem2* pocetnoStanje, *trenutnoStanje, *krajnjeStanje;

	Elem* prvi, * posl;
	void dodajNaListu(Elem* nov);
public:


	SubtitleEditor();
	~SubtitleEditor();

	int loadSubtitles(string subtitle);
	string saveSubtitles();

	void addToSelection(int start, int end);
	void deselect();

	void boldText(int start, int end);
	void italicText(int start, int end);
	void underlineText(int start, int end);
	void removeStyle();

	void shiftForward(int ms);
	void shiftBackward(int ms);
	void insertSubtitle(int start, int length, string text);
	void merge(string subtitles);
	void deleteSubtitles();

	void fixLongLines(int max);
	void fixLongTime(int ms);

	void undo();
	void redo();
};


#include "SubtitleEditor.h"
#include "Time.h"

int SubtitleEditor::isNumber(char c) {
	if (c <= '9' && c >= '0')return 1;
	return 0;
}

int SubtitleEditor::loadSubtitle(string subtitles) {
	if (prvi) delete prvi;
	prvi = posl = nullptr;
	int index = 0;
	std::string id = "", vreme1 = "", vreme2 = "", tekst = "";
	std::string trenutni = "";
	int k = 0;
	while (index<(subtitles.length())) { // while(index < subtitles.length())

		while (subtitles[index] != '\n') {
			trenutni += subtitles[index];
			index++;
		}
		id = trenutni;
		trenutni = "";
		index++;
		while (subtitles[index] != ' ') {
			trenutni += subtitles[index];
			index++;
		}
		vreme1 = trenutni;
		trenutni = "";
		index += 5; // time1 --> time2
		while (subtitles[index] != '\n') {
			trenutni += subtitles[index];
			index++;
		}
		vreme2 = trenutni;
		trenutni = "";
		index++;
		
		while ((subtitles[index]!='\n' || subtitles[index+1]!='\n') && index<(subtitles.length() - 1)) {
			trenutni += subtitles[index];
			index++;
			if (index > subtitles.length() - 3)break;
		}
		tekst = trenutni;
		trenutni = ""; //vreme1 <- string
		Time* v1 = Time::strToTime(vreme1);
		Time* v2 = Time::strToTime(vreme2);
		int id1 = stoi(id);
		Elem* nov = new Elem(id1, v1, v2, tekst);
		dodajNaListu(nov);
		index += 2;
	}
	return 1;
}

void SubtitleEditor::dodajNaListu(Elem* nov) {
	if (!prvi) prvi = posl = nov;
	else posl = posl->next = nov;
}

string SubtitleEditor::saveSubtitle() {
	Elem* pom = prvi;
	std::string ret;
	while (pom != nullptr) {
		ret += std::to_string(pom->id);
		ret += "\n";
		ret += pom->startTime->timeToStr();
		ret += " --> ";
		ret += pom->endTime->timeToStr();
		ret += "\n";
		ret += pom->tekst;
		ret += "\n";
		ret += "\n";
		pom = pom->next;
	}
	ret += "\n";
	return ret;
}

void SubtitleEditor::addToSelection(int start,int end) {
	if (start < 1) start = 1;
	if (end > posl->id)end = posl->id;

	Elem* pom = prvi;
	while (pom) {
		if (pom->id >= start && pom->id <= end) pom->selected = true;
		pom = pom->next;
	}
}

void SubtitleEditor::deselect() {
	Elem* pom = prvi;
	while (pom) {
		if(pom->selected)pom->selected = false;
		pom = pom->next;
	}
}

string SubtitleEditor::fontTekst(string tekst,int start, int end,char f) {
	//Ja sam <b>car</b> lazar , start=7 end 10
	//if (tekst == "")return tekst;
	if (start < 0) start = 0;
	if (end > tekst.length() - 1)end = tekst.length() - 1;
	
	string noviTekst = "";
	string openTag = "<" + string(1, f); openTag += ">";
	string closeTag = "</" + string(1, f); closeTag += ">";
	for (int i = 0; i <= start; i++)noviTekst += tekst[i]; //Ja sam 
	noviTekst += openTag; //Ja sam <b> 
	for (int i = start + 1; i < end + 1; i++)noviTekst += tekst[i];//Ja sam <b> car 
	noviTekst += closeTag;
	for (int i = end + 1; i < (tekst.length() + 1); i++)noviTekst += tekst[i];
	return noviTekst;
}
void SubtitleEditor::fontOpseg(int start, int end,char f) {
	Elem* pom = prvi;
	while (pom) { // prolazim kroz listu i font-ujem selektovane
		if (pom->selected) {
			pom->tekst = fontTekst(pom->tekst,start,end,f);
		}
		pom = pom->next;
	}
}


void SubtitleEditor::boldText(int start, int end) {
	fontOpseg(start, end, 'b');
}
void SubtitleEditor::italicText(int start, int end) {
	fontOpseg(start, end, 'i');
}
void SubtitleEditor::underlineText(int start, int end) {
	fontOpseg(start, end, 'u');
}
string SubtitleEditor::removeStyleText(string tekst) {
	string buf = tekst;
	int idx = 0;
	int opened = 0;
	for (int i = 0; i < buf.length(); i++) {
		if (buf[i] == '<') opened = 1;
		else if (buf[i] == '>')opened = 0;
		else if (!opened)buf[idx++] = buf[i];
	}
	string ret = "";
	for (int i = 0; i < idx; i++)ret += buf[i];
	return ret;
}
void SubtitleEditor::removeStyle() {
	Elem* pom = prvi;
	while (pom) {
		if (pom->selected) {
			pom->tekst = removeStyleText(pom->tekst);
		}
		pom = pom->next;
	}
}

























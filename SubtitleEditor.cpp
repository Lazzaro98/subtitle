#include "SubtitleEditor.h"
#include "Time.h"

int SubtitleEditor::isNumber(char c) {
	if (c <= '9' && c >= '0')return 1;
	return 0;
}

int SubtitleEditor::loadSubtitle(string subtitles) {
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
		
		while (!isNumber(subtitles[index + 2]) && index<(subtitles.length() - 1)) {
			trenutni += subtitles[index];
			index++;
			if (index > subtitles.length() - 3)break;
		}
		tekst = trenutni;
		trenutni = ""; //vreme1 <- string
		Time* v1 = new Time(0,0,0,0);
		Time* v2 = new Time(0,0,0,0);
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
#include "SubtitleEditor.h"

void SubtitleEditor::unloadSubtitle() {
	if (prvi)delete prvi;
	prvi = posl = nullptr;
}
SubtitleEditor::SubtitleEditor() {
	prvi = posl = nullptr;
}

SubtitleEditor::~SubtitleEditor() {
	unloadSubtitle();
}
void SubtitleEditor::dodajNaListu(Elem* nov) {
	if (!prvi) prvi = posl = nov;
	else posl = posl->next = nov;
}


int SubtitleEditor::loadSubtitle(string subtitles) {
	unloadSubtitle();

	int index = 0;
	std::string id = "", vreme1 = "", vreme2 = "", tekst = "";
	std::string trenutni = "";//ovo nam je bafer za citanje. Dakle idemo kroz fajl i citamo do neke pozicije. Kada smo stigli npr. do novog reda, tada ispraznimo ovaj bafer tako sto upisemo to sto smo procitali u neku drugu promenljivu, npr id, a bafer setujemo na prazan da bismo mogli da citamo dalje
	while (index < (subtitles.length()-1)) { // citamo do kraja fajla
		//pocetak fajla
		while (subtitles[index] != '\n') { //citamo do prvog znaka za novi red, jer nam je dotle ID
			trenutni += subtitles[index]; // dodajemo po jedan karakter tog ID-a, sve dok ne dodjemo do novog reda
			index++;
		}
		//kada smo dosli do novog reda, u id upisemo to sto smo procitali
		id = trenutni;
		//setujemo bafer na prazan, da mozemo sada da citamo sledecu stavku (vreme)
		trenutni = "";
		//povecamo index za 1 da bismo preskocili znak za novi red ('\n')
		index++;

		while (subtitles[index] != ' ') { //citamo sve do prvog blanko znaka (to nam je startTime)
			trenutni += subtitles[index];
			index++;
		}
		//kada smo procitali, upisujemo sve iz bafera u vreme1
		vreme1 = trenutni;
		//setujemo bafer na prazan
		trenutni = "";
		//index uvecavamo za 5 da bismo preskocili  " ---> " 
		index += 5;

		//sada citamo endTime, do znaka za novi red
		while (subtitles[index] != '\n') {
			trenutni += subtitles[index];
			index++;
		}
		//ovo je isto kao gore
		vreme2 = trenutni;
		trenutni = "";
		index++; // preskacemo znak za novi red '\n'


		//Citamo dok ne naidjemo na dva uzastopna znaka za novi red, ili dok ne prekoracimo duzinu fajla
		while ((subtitles[index] != '\n' || subtitles[index + 1] != '\n') && index < (subtitles.length() - 1)) {
			trenutni += subtitles[index];
			index++;
			//if (index > subtitles.length() - 3)break;
		}
		tekst = trenutni;
		trenutni = "";

		//proverimo potencijalne greske u formatu
		int id1 = stoi(id); // id smo gore cuvalli kao string, a treba nam kao integer. stoi(string s), konvertuje string u int
		if (!Time::isValid(vreme1 + " --> " + vreme2) || id1<1) { //ako vreme nije validna, vrati id subtitle-a koji ima gresku u vremenu, ili je id<1
			unloadSubtitle(); //unloadujemo sve sto smo do sada ucitali
			return id1; //vratmo redni broj titla koji ima gresku
		}

		//Sada kad smo procitali ceo jedan element, treba da upisemo te podatke lepo u nasu listu
		Time* v1 = Time::strToTime(vreme1);//vreme iz string konvertujemo u Time (lakse nam je da cuvamo vreme kao int da bismo mogli da operacije da izvrsavamo nad njim (sabiranje,oduzimanje etc.))
		Time* v2 = Time::strToTime(vreme2);
		Elem* nov = new Elem(id1, v1, v2, tekst); //samo napravimo nas Elem sada
		dodajNaListu(nov); // i dodamo ga u listu
		index += 2; // povecamo index za dva, da preskocimo dva uzastopna znaka za novi red ('\n')
	}
	return -1;
}


string SubtitleEditor::saveSubtitle() {
	Elem* pom = prvi;//krenemo od prvog elementa
	std::string ret;
	//pratimo opisani format u tekstu zadatka
	while (pom != nullptr) { //ukoliko element postoji
		ret += std::to_string(pom->id); //na ret nalepi id
		ret += "\n";//zatim stavi novi red
		ret += pom->startTime->timeToStr();//zatim nalepi pocetno vreme
		ret += " --> ";//zatim nalepi -->
		ret += pom->endTime->timeToStr();//zatim nalepi krajnje vreme
		ret += "\n";//onda ponovo novi red
		ret += pom->tekst; //nalepi tekst
		ret += "\n";
		ret += "\n";
		pom = pom->next;//idi na sledeci element
	}
	ret += "\n"; ret += '\n'; ret += '\n';
	return ret;
}


void SubtitleEditor::addToSelection(int start, int end) {
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
		if (pom->selected)pom->selected = false;
		pom = pom->next;
	}
}

string SubtitleEditor::fontTekst(string tekst, int start, int end, char f) { //ova funkcija ce nad stringom stekst, pocevski od start pa do end pozicije, dodati <f> tag
	//Zdravo svima. -> fontTekst("Zdravo svima.",7,11,'b') = "Zdravo <b>svima</b>"
	if (start < 0 || start >tekst.length()-1) start = 0;
	if (end > tekst.length() - 1 || end<0)end = tekst.length() - 1;

	string noviTekst = "";
	//HTML open i close tag koji treba da dodamo: da li je to <b> ili <i> ili stagod, u zavisnosti od onoga sta je funkciji prosledjeno
	string openTag = "<" + string(1, f); openTag += ">"; //string(1,f); ce konvertovati karakter f u string i nalepice ga na openTag
	string closeTag = "</" + string(1, f); closeTag += ">";// isto kao prethodno

	int otv = 0,zatv=0;//otv broji '<', a zatv broji '>'. Ukoliko su oba parna, sve je okej i mozemo stilizovati. Ako ne, greska.

	for (int i = 0; i < start; i++) { 	//pratimo primer koji je gore naveden. Do pozicije starta necemo nista da formatiramo pa cemo samo prepisati tekst. noviTekst = "Zdravo "
		noviTekst += tekst[i];
		if (tekst[i] == '<') otv++;
		if (tekst[i] == '>') zatv++;
	}

	noviTekst += openTag; //Sada kad smo stigli do start-a, nalepimo tag. noviTekst = "Zdravo <b>"
	if (otv%2!=0 || zatv%2!=0) return "ERROR: GRESKA_U_STILIZOVANJU";
	for (int i = start; i < end + 1; i++)//sada sve do end-a, ponovo prepisujemo sve. noviTekst = "Zdravo <b>svima"
	{
		noviTekst += tekst[i];
		if (tekst[i] == '<') return "ERROR: GRESKA_U_STILIZOVANJU";
	}

	noviTekst += closeTag; //kada smo stigli do end-a, samo zatvorimo tag. noviTekst = "Zdravo <b>svima</b>"
	
	for (int i = end + 1; i < (tekst.length() + 1); i++)noviTekst += tekst[i]; //ukoliko nakon end-a ima jos teksta samo cemo ga prepisati. U nasem primeru nema vise teksa
	
	
	return noviTekst; //uspesno smo formatirali jej
}

void SubtitleEditor::fontSelektovane(int start, int end, char f) {
	Elem* pom = prvi;
	while (pom) { // prolazim kroz listu 
		if (pom->selected) {//i font-ujem selektovane
			pom->tekst = fontTekst(pom->tekst, start, end, f); //menjam tekst titla sa stilizovanim tekstom, tako sto samo pozovemo funkciju koja stilizuje string
		}
		pom = pom->next;
	}
}

void SubtitleEditor::boldText(int start, int end) {
	fontSelektovane(start, end, 'b');
}
void SubtitleEditor::italicText(int start, int end) {
	fontSelektovane(start, end, 'i');
}
void SubtitleEditor::underlineText(int start, int end) {
	fontSelektovane(start, end, 'u');
}


string SubtitleEditor::removeStyleText(string tekst) {
	string buf = tekst; //krenemo od pocetnog teksta, koji sadrzi TAG-ove
	int idx = 0;//krenemo od nule
	int opened = 0;//belezimo da li smo naisli na open tag znak. Na pocetku ne
	for (int i = 0; i < buf.length(); i++) { // prolazimo kroz tekst
		if (buf[i] == '<') opened = 1; // ako smo naisli na open zabelezimo
		else if (buf[i] == '>')opened = 0; // ako smo naisli na close zabelezimo
		else if (!opened)buf[idx++] = buf[i]; // ako nam je open false, to znaci da citamo obican tekst(ne tag), tako da cemo samo prepisati. Ako je open true, to znaci da citamo tag, i  u tom slucaju necemo uraditi nista, jer zelimo da izignorisemo(ne procitamo) sve tagove.
	}
	string ret = ""; //Na kraju cemo na jedan string da nalepimo sve sto smo procitali. Ovo mora jer duzina string-a se ne smanji kada izbacimo sve tagove. Zato cemo do idx-a, da nalepimo sve na novi string.
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

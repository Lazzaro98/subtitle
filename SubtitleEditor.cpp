#include <iostream>
#include "Time.h"
#include "SubtitleEditor.h"
#include <sstream>
#include <fstream>
using namespace std;

//--------------------------------Glavne funkcije(one koje se zahtevaju u zadatku)-------------------------------------------------//

SubtitleEditor::SubtitleEditor() {
	prvi = posl = nullptr;
	pocetnoStanje = trenutnoStanje = krajnjeStanje = nullptr;
}

SubtitleEditor::~SubtitleEditor() {
	unloadSubtitles();
}

int SubtitleEditor::loadSubtitles(string subtitles) {
	unloadSubtitles();

	int index = 0;
	string id = "", vreme1 = "", vreme2 = "", tekst = "";
	string trenutni = "";//ovo nam je bafer za citanje. Dakle idemo kroz fajl i citamo do neke pozicije. Kada smo stigli npr. do novog reda, tada ispraznimo ovaj bafer tako sto upisemo to sto smo procitali u neku drugu promenljivu, npr id, a bafer setujemo na prazan da bismo mogli da citamo dalje
	while (index < (subtitles.length() - 1)) { // citamo do kraja fajla
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
		if (!Time::isValid(vreme1 + " --> " + vreme2) || id1 < 1) { //ako vreme nije validna, vrati id subtitle-a koji ima gresku u vremenu, ili je id<1
			//unloadSubtitles(); //unloadujemo sve sto smo do sada ucitali
			return id1; //vratmo redni broj titla koji ima gresku
		}

		//Sada kad smo procitali ceo jedan element, treba da upisemo te podatke lepo u nasu listu
		Time* v1 = Time::strToTime(vreme1);//vreme iz string konvertujemo u Time (lakse nam je da cuvamo vreme kao int da bismo mogli da operacije da izvrsavamo nad njim (sabiranje,oduzimanje etc.))
		Time* v2 = Time::strToTime(vreme2);
		Elem* nov = new Elem(id1, v1, v2, tekst); //samo napravimo nas Elem sada
		dodajNaListu(nov); // i dodamo ga u listu
		index += 2; // povecamo index za dva, da preskocimo dva uzastopna znaka za novi red ('\n')
	}
	dodajStanje(prvi,posl);
	return -1;
}

string SubtitleEditor::saveSubtitles() {
	Elem* pom = prvi;//krenemo od prvog elementa
	string ret = "";
	//pratimo opisani format u tekstu zadatka
	while (pom != nullptr) { //ukoliko element postoji
		ret += to_string(pom->id); //na ret nalepi id
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

	return ret;
}

void SubtitleEditor::addToSelection(int start, int end) {
	if (start < 1) start = 1;
	if (end > posl->id)end = posl->id;

	Elem* pom = prvi;
	while (pom != nullptr) {
		if (pom->id >= start && pom->id <= end) pom->selected = true;
		pom = pom->next;
	}
	dodajStanje(prvi, posl);
}

void SubtitleEditor::deselect() {

	Elem* pom = prvi;
	while (pom) {
		if (pom->selected)pom->selected = false;
		pom = pom->next;
	}
	dodajStanje(prvi, posl);
}


void SubtitleEditor::boldText(int start, int end) {
	fontSelektovane(start, end, 'b');
	dodajStanje(prvi, posl);
}
void SubtitleEditor::italicText(int start, int end) {
	fontSelektovane(start, end, 'i');
	dodajStanje(prvi, posl);
}
void SubtitleEditor::underlineText(int start, int end) {
	fontSelektovane(start, end, 'u');
	dodajStanje(prvi, posl);
}


void SubtitleEditor::removeStyle() {
	Elem* pom = prvi;
	while (pom) {
		if (pom->selected) {
			pom->tekst = removeStyleText(pom->tekst);
		}
		pom = pom->next;
	}
	dodajStanje(prvi, posl);
}



void SubtitleEditor::shiftForward(int ms) {
	Time* time = Time::msecToTime(ms); //pretvaramo vreme u milisekunde, jer to odgovara fji za sabiranje, koja je prethodno objasnjena
	Time* novo = nullptr;
	Elem* pok = prvi;

	bool imaSelekt = imaSelektovanih();

	while (pok) {
		if (((imaSelekt && pok->selected) || !imaSelekt)) {
			novo = Time::sabiranjeVremena(time, pok->endTime); //azuriramo krajnje vreme
			pok->endTime = novo;
			novo = Time::sabiranjeVremena(time, pok->startTime); //azuriramo pocetno vreme
			pok->startTime = novo;
		}
		pok = pok->next;
	}

	dodajStanje(prvi, posl);
}


void SubtitleEditor::shiftBackward(int ms) { //isti princip kao kod sabiranja

	Time* time = Time::msecToTime(ms);
	Elem* pok = prvi;
	Elem* pom = NULL; //prethodnik
	Time* novo = nullptr;
	bool imaSelekt = imaSelektovanih();

	while (pok) {
		if (((imaSelekt && pok->selected) || !imaSelekt)) {
			novo = Time::oduzimanjeVremena(pok->startTime, time);
			pok->startTime = novo;
			novo = Time::oduzimanjeVremena(pok->endTime, time);
			pok->endTime = novo;

		}
		pok = pok->next;
	}

	dodajStanje(prvi, posl);
}

void SubtitleEditor::insertSubtitle(int start, int
	length, string text) {

	if (text[text.length() - 1] == '\n') {
		string noviTekst = "";
		for (int i = 0; i < text.length()-1; i++)
			noviTekst += text[i];
		text = noviTekst;
	}

	Time* sTime = Time::msecToTime(start); //pravimo pocetno vreme
	Time* eTime = Time::msecToTime(start + length); // pravimo krajnje vreme


	if (Time::prviManji(sTime, prvi->startTime) == true) { //pitamo da li je nase vreme pre pocetnog vremena, da bi azurirali pokazivac prvi
		if (Time::prviManji(prvi->startTime, eTime) || (prvi->startTime == eTime)) return; //preklapanje?
		Elem* novi = new Elem(1, sTime, eTime, text, prvi); // ako jeste pravimo elem od zadatog vremena, koji cemo ubaciti na pocetak liste
		prvi = novi;
		novi = prvi->next;
		while (novi) { //azuriranje id-a
			novi->id++;
			novi = novi->next;
		}
		return;
	}

	Elem* pok = prvi;

	while (pok->next) { //ovo je kada se ne dodaje na pocetak liste
		if (Time::prviManji(sTime, pok->next->startTime) == true) {
			if (Time::prviManji(pok->next->startTime, eTime) || (Time::timeToMsec(pok->next->startTime) == Time::timeToMsec(eTime)) || (Time::prviManji(sTime, pok->endTime)) || (Time::timeToMsec(sTime) == Time::timeToMsec(pok->endTime))) return;
			Elem* novi = new Elem(pok->next->id, sTime, eTime, text, pok->next);
			pok->next = novi;
			novi = novi->next;
			while (novi) { //azuriranje id-a
				novi->id++;
				novi = novi->next;
			}
			return;
		}
		pok = pok->next;
	}

	if (Time::prviManji(posl->endTime, sTime) == true) { //ovaj slucaj gde smo obisli celu listu i shvatili smo da ovaj titl treba na kraj, radimo zbog azuriranja pokazivaca posl
		int id = posl->id + 1;
		Elem* novi = new Elem(id, sTime, eTime, text, nullptr);
		posl->next = novi;
		posl = novi;
		return;
	}
	dodajStanje(prvi, posl);
}

void SubtitleEditor::deleteSubtitles() {
	Elem* pok = prvi;

	while (pok->next) { //idemo kroz listu i trazimo prvi selektovani

		if (pok->selected) {
			del(prvi, pok->id); //pomocnoj fji za brisanje prosledjuemo pocetak liste, tj govorimo joj na koju smo listu mislili i od kog to id-a je krenula selekcija
			pok = prvi;
		}
		else pok = pok->next;
	}

	if (prvi == nullptr) //izbrisali celu listu
		return;

	if ((prvi->next == nullptr) && (prvi->next->selected)) { //brisemo jedini preostali element
		delete prvi;
		prvi = posl = nullptr;
	}

	// Trazimo pretposlednji elem u slucaju da poslednji mora da se brise (imamo listu sa vise elemenata)
	Elem* second_last = prvi;
	while (second_last->next->next != nullptr)
		second_last = second_last->next;

	// Brisemo ga i azuriramo posl 
	if (second_last->next->selected) {
		delete (second_last->next);
		posl = second_last;
		second_last->next = nullptr;
	}
	//azuriramo ovo jer je doslo do brisanja 

	updateIDs();
	dodajStanje(prvi, posl);
}

void SubtitleEditor::merge(string subtitles) {
	string subtitlesPreth = saveSubtitles(); //moramo da cuvamo listu u vidu stringa to radi fja saveSubtitles(), jer fja loadSubtitles brise prethodne titlove, a mi ne zelimo da ih izgubimo
	int l = subtitlesPreth.length(); // treba nam duzina ovog titla jer zelimo da uklonimo poslednji /n, jer on na kraju fajla ima dva /n koji su inace oznaka kraja fajla, a to zelimo da izbegnemo
	string subtitles1 = "";
	for (int i = 0; i < l; i++) { // zato trazimo da procita l-1
		subtitles1 += subtitlesPreth[i]; // modifikovani prethodni string
	}
	/*char c1 = subtitles1[subtitles1.length() - 3];
	char c2= subtitles1[subtitles1.length() - 2];
	char c3 = subtitles1[subtitles1.length() - 1];*/
	string subtitlesSpojen = ""; // sada spajamo zadati string i nas string
	subtitlesSpojen += subtitles1;
	subtitlesSpojen += subtitles;

	std::ofstream out("output_merge.txt");
	out << subtitles1;
	out.close();

	loadSubtitles(subtitlesSpojen); //od spojenog stringa pravimo listu 
	updateIDs(); //azuriramo id svakog elementa

	dodajStanje(prvi, posl);
}


void SubtitleEditor::fixLongLines(int max) {
	bool imaSelekt = imaSelektovanih();
	Elem* pom = prvi;
	while (pom) {
		if (imaSelekt && pom->selected) pom->tekst = fixLongLines1(pom->tekst, max);
		else if (!imaSelekt) pom->tekst = fixLongLines1(pom->tekst, max);
		pom = pom->next;
	}
	dodajStanje(prvi, posl);
}


void SubtitleEditor::fixLongTime(int ms) {
	Elem* preth = prvi, * pok = prvi->next;
	bool imaSelekt = imaSelektovanih();
	if (Time::trajanje(preth->startTime, preth->endTime) > ms && ((imaSelekt && preth->selected) || !imaSelekt)) {
		Elem* pom = podeliSubtitle(preth, ms);
		preth->next = pom;
		Elem* pom2 = pom; while (pom2->next)pom2 = pom2->next;
		preth = pom2;
		pom2->next = pok->next;
		prvi = pom;
	}
	posl = prvi;
	while (pok) {
		if (Time::trajanje(pok->startTime, pok->endTime) > ms && ((imaSelekt && pok->selected) || !imaSelekt)) {
			Elem* pom = podeliSubtitle(pok, ms);
			Elem* obrisi = pok;
			preth->next = pom;
			while (pom->next) pom = pom->next;
			//preth = pom;
			preth = pom;
			pom->next = pok->next;
			//preth = pok; //pok = pok->next->next;
			pok = preth->next;
		}
		else
		{
			pok = pok->next;
			preth = preth->next;
		}
	}
	updateIDs();
	dodajStanje(prvi, posl);
}

void SubtitleEditor::undo() {
	if (trenutnoStanje->prev)
	{
		trenutnoStanje = trenutnoStanje->prev;
	}
	prvi = trenutnoStanje->stanjePrvi;
	posl = trenutnoStanje->stanjePosl;
}

void SubtitleEditor::redo() {
	if (trenutnoStanje->next)
	{
		trenutnoStanje = trenutnoStanje->next;
	}

	prvi = trenutnoStanje->stanjePrvi;
	posl = trenutnoStanje->stanjePosl;
}






//------------------------------------Pomocne funkcije(one smo napisali, ali se ne zahtevaju eksplicitno)--------------------------//

void SubtitleEditor::dodajNaListu(Elem* nov) {
	if (!prvi) prvi = posl = nov;
	else posl = posl->next = nov;
}

void SubtitleEditor::unloadSubtitles() {
	Elem* pom = prvi;
	while (pom) {
		Elem* obrisi = pom;
		pom = pom->next;
		delete obrisi;
		obrisi = nullptr;
	}
	prvi = posl = nullptr;
}


string SubtitleEditor::fontTekst(string tekst, int start, int end, char f) { //ova funkcija ce nad stringom stekst, pocevski od start pa do end pozicije, dodati <f> tag
	//Zdravo svima. -> fontTekst("Zdravo svima.",7,11,'b') = "Zdravo <b>svima</b>"
	/*if (f != 'b' || f != 'u' || f != 'i') {
		return "ERROR: GRESKA_U_STILIZOVANJU";
	}*/
	if (start < 0 || start >tekst.length() - 1) start = 0;
	if (end > tekst.length() - 1 || end < 0)end = tekst.length() - 1;

	string noviTekst = "";
	//HTML open i close tag koji treba da dodamo: da li je to <b> ili <i> ili stagod, u zavisnosti od onoga sta je funkciji prosledjeno
	string openTag = "<" + string(1, f); openTag += ">"; //string(1,f); ce konvertovati karakter f u string i nalepice ga na openTag
	string closeTag = "</" + string(1, f); closeTag += ">";// isto kao prethodno
	//<b>  </b>
	int otv = 0, zatv = 0;//otv broji '<', a zatv broji '>'. Ukoliko su oba parna, sve je okej i mozemo stilizovati. Ako ne, greska.
	// <b>Cao svima</b> <i>ja sam lazar</i> <u>kako si? </u>
	for (int i = 0; i < start; i++) { 	//pratimo primer koji je gore naveden. Do pozicije starta necemo nista da formatiramo pa cemo samo prepisati tekst. noviTekst = "Zdravo "
		noviTekst += tekst[i];
		if (tekst[i] == '<') otv++;
		if (tekst[i] == '>') zatv++;
	}

	noviTekst += openTag; //Sada kad smo stigli do start-a, nalepimo tag. noviTekst = "Zdravo <b>"
	if (otv % 2 != 0 || zatv % 2 != 0) return "ERROR: GRESKA_U_STILIZOVANJU";
	for (int i = start; i < end + 1; i++)//sada sve do end-a, ponovo prepisujemo sve. noviTekst = "Zdravo <b>svima"
	{
		noviTekst += tekst[i];
		if (tekst[i] == '<') return "ERROR: GRESKA_U_STILIZOVANJU";
	}

	noviTekst += closeTag; //kada smo stigli do end-a, samo zatvorimo tag. noviTekst = "Zdravo <b>svima</b>"

	for (int i = end + 1; i < (tekst.length()); i++)noviTekst += tekst[i]; //ukoliko nakon end-a ima jos teksta samo cemo ga prepisati. U nasem primeru nema vise teksa


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



void SubtitleEditor::del(Elem* pocetak, int selektId)
{
	Elem* temp = pocetak;
	Elem* prev = NULL; //koristimo prethodnik da bi kada izbrisemo odgovarajuci element, povezemo prethodnika i sledbenika i tako ne remetimo povezanost liste izbacivanjem elementa

	if (temp != NULL && temp->id == selektId) //ovaj deo koda je sta radimo ako se selekcija odnosila na prvi element
	{
		prvi = pocetak = temp->next; //Sta radimo kada brisemo prvi elem liste
		delete temp;
		temp = nullptr;
		return;
	}
	while (temp != NULL && temp->id != selektId) // ustanovili smo da prvi elem nije selektovan i sada prolazimo kroz listu dok ne dodjemo do elementa koji jeste selektovan
	{
		prev = temp;
		temp = temp->next;
	}
	if (temp == NULL) //ako smo zavrsili sa obilaskom liste, izlazimo iz fje
		return;
	prev->next = temp->next; //ovde smo ako je while pukao i nismo prosli celu listu, vec smo pronasli odgovarajuci id, tj selektovani elem
	delete temp;// brisemo ga
}



bool SubtitleEditor::shouldFix(string subtitle, int max) {
	//Citamo od pocetka stringa, 1 po 1 karakter.
	//Imacemo 2 integer-a. Jedan ce voditi evidenciju dokle smo stigli u celom stringu(to ce biti index),
	//a drugi ce voditi evidenciju dokle smo stigli u redu u kom se trenutno nalazimo(to ce biti curLineIndex - current Line Index)
	//Kada god dodjemo do znaka za novi red, curLineIndex se resetuje na 0
	int index = 0, curLineIndex = 0, brojac = 0;
	while (index < subtitle.length()) {
		if (subtitle[index] == '\n') { curLineIndex = 0; brojac = 0; }
		else if (brojac > max) return true; //posto ce nama curLineIndex da vodi racuna o tome na kojoj se poziciji trenutno nalazimo u tom redu, ukoliko ona ikada predje vrednost max, to znaci da cemo morati da secemo! Funkcija odma vraca true.
		index++; 
		curLineIndex++; // idemo na sledeci karakter
		if (subtitle[index] != ' ')brojac++;
	}
	return false; // Ako gore nikada nismo vratili true, znaci da nikada curLineIndex nije presao vrednost max, i ne treba nista da se sece.
}

string* SubtitleEditor::wordsFromSubtitle(string subtitle, int* brojReci) {//funkcija ce izbrojiti reci usput, i to sto izbroji upisace u brojReci. 
	string* reci = new string[1000];
	int index = 0, rbrReci = 0;//index gde se trenutno nalazimo u subtitle-u, i redni broj reci koju citamo trenutno
	string trenutni = "";//ovim citamo do blanko znaka, i kada stignemo do blanko znaka sve sto smo procitali stavimo u niz Reci, a trenutni resetujemo na prazno da bismo mogli ostale reci da citamo na isti nacin
	while (index < subtitle.length()) {
		/*if (subtitle[index] == '.' || subtitle[index] == ',') {
			reci[rbrReci] = trenutni;
			trenutni = "";
			rbrReci++;
			if (index < subtitle.length() - 1) {
				if (subtitle[index + 1] == ' ' || subtitle[index + 1] == '\n')
					index++;
			}
		}
		else */ //Uzimamo rec sa sve tackom ili zarezom. To radimo da bismo sacuvali mesta na kojima je tacka/zarez. Kada bi se odkomentarisao deo iznad, funkcija bi izdvajala sve reci, bez zareaz i tacaka, i pazila bi na to da neki pisci subtitl-ova naprave gresku pripisanju i ne stave razmak posle tacke/zareza
		if (subtitle[index] == '\n' || subtitle[index] == ' ') {
			reci[rbrReci] = trenutni;
			trenutni = "";
			rbrReci++;
		}
		else
			trenutni += subtitle[index];
		index++;
	}
	if (trenutni != "") {
		reci[rbrReci] = trenutni;

	}
	*brojReci = rbrReci + 1;
	return reci;
}

string SubtitleEditor::fixLongLines1(string subtitle, int max) {
	if (!shouldFix(subtitle, max))return subtitle;
	int brojReci = 0;
	string* reci = wordsFromSubtitle(subtitle, &brojReci);
	int ind = 0;
	string noviString = "";
	int trenutnaDuzina = 0;
	while (ind < brojReci) {
		if (trenutnaDuzina + reci[ind].length() <= max) {
			if (noviString != "") {
				if (noviString[noviString.length() - 1] != '\n')
					noviString += " ";
				//trenutnaDuzina++;
			}

			noviString += reci[ind];
			trenutnaDuzina += reci[ind].length();
			ind++;
		}
		else {
			if (reci[ind].length() > max) return " ERROR: DUZINA_RECI_VECA_OD_POSTAVLJENOG_MAX";
			trenutnaDuzina = 0;
			noviString += "\n";
		}
	}
	return noviString;

}

bool SubtitleEditor::imaSelektovanih() {
	Elem* pom = prvi;
	while (pom) {
		if (pom->selected)return true;
		pom = pom->next;
	}
	return false;
}

void SubtitleEditor::updateIDs() {
	Elem* pom = prvi;
	int index = 1;
	while (pom) {
		pom->id = index;
		pom = pom->next;
		index++;
	}
}
SubtitleEditor::Elem* SubtitleEditor::podeliSubtitle(Elem* elem, int ms) {
	Elem* nov = nullptr, * novPosl = nullptr;
	int brojReci = 0;
	string* reci = wordsFromSubtitle(elem->tekst, &brojReci);
	int duration = Time::trajanje(elem->startTime, elem->endTime);
	int n = duration / ms;
	if (duration % ms != 0) n++;
	int brojReciPoTitlu = brojReci / n;
	int visakReci = brojReci % n;
	int trajanjeJednogSubtitle = duration / n;
	string noviTekst = "";
	Time* startTime = elem->startTime;
	Time* x = Time::msecToTime(trajanjeJednogSubtitle);
	Time* endTime = Time::sabiranjeVremena(startTime, Time::msecToTime(trajanjeJednogSubtitle));
	int ind = 0;

	for (int i = 0; i < n; i++) {
		string text = "";
		for (int j = 0; j < brojReciPoTitlu; j++) {
			if (j == (brojReciPoTitlu - 1))text += reci[ind++];
			else text += reci[ind++]+" ";
		}
		if (visakReci-- > 0) text += " "+reci[ind++];
		Elem* noviZaNalepi = new Elem(0, startTime, endTime, text);
		if (noviZaNalepi->tekst != "") {
			if (!nov) nov = novPosl = noviZaNalepi;
			else novPosl = novPosl->next = noviZaNalepi;
		}
		startTime = endTime;
		endTime = Time::sabiranjeVremena(endTime, Time::msecToTime(trajanjeJednogSubtitle));
	}
	return nov;
}



void SubtitleEditor::dodajStanje(Elem* stanjePocetni1, Elem* stanjeKrajnji1) {
	
	Elem* stanjeKrajnjiR=nullptr; // poslednji element u jednom stanju (prvi i posl)
	Elem* stanjePocetni = napraviKopijuListe(stanjePocetni1,stanjeKrajnjiR);
	if (!pocetnoStanje) {
		pocetnoStanje = krajnjeStanje = trenutnoStanje = new SubtitleEditor::Elem2(stanjePocetni,stanjeKrajnjiR,nullptr, nullptr);
	}
	else {
		trenutnoStanje->next = new SubtitleEditor::Elem2(stanjePocetni,stanjeKrajnjiR, trenutnoStanje, nullptr);
		trenutnoStanje = trenutnoStanje->next;
	}

}

SubtitleEditor::Elem* SubtitleEditor::napraviKopijuListe(Elem* prvi, Elem* lastr) {
	Elem* pom = prvi;
	Elem* first = nullptr, *last=nullptr;
	while (pom) {
		if(!first)first = last = new Elem(pom->id, pom->startTime, pom->endTime, pom->tekst);
		else last=last->next= new Elem(pom->id, pom->startTime, pom->endTime, pom->tekst);
		pom = pom->next;
	}
	lastr = last;
	return first;
}

/*
*
*/
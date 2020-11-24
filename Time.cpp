#include "Time.h"
using namespace std;

std::string Time::timeToStr() { //Konvertujemo objekat tipa Time u string
	//objekat Time npr izgleda ovako 01:21:05:050
	string ret = ""; //na pocetku string je prazan
	if (this->h / 10 == 0) ret += to_string(0); //Ako je broj h jednocifren, to zna;i da moramo da dodamo nulu na poetku stringa. ret="0"
	ret += to_string(h);// konvertujemo h u string i nalepimo ga na string. ret="01"
	ret += ":";//ret="01:"


	if (min / 10 == 0) ret += to_string(0); //isto kao za h, s tim da ovde, u nasem primeru, necemo nalepiti nula jer je broj dvocifren. ret="01:"
	ret += to_string(min); //isto kao za h. ret="01:21"
	ret += ":";// ret="01:21:"

	if (sec / 10 == 0) ret += to_string(0); //nalepljujemo nulu jer je broj jednocifren. ret="01:21:0"
	ret += to_string(sec); //nalepljujemo broj. ret="01:21:05"
	ret += ","; //ret = "01:21:05,"

	if (msec / 100 == 0)ret += to_string(0); //Nalepljujemo nulu jer je broj dvocifren. // ret = "01:21:05,0"
	if (msec / 10 == 0)ret += to_string(0);//Ovo pita da li je broj jednocifren, pa da nalepi jos jednu nulu. U nasem primeru nece nista uraditi. ret = "01:21:05,0"
	ret += to_string(msec); // ret = "01:21:05,050"
	return ret;//Uspesno smo konvertovali Time u string, jej

}

Time* Time::strToTime(std::string time) {
	int h, min, sec, msec;//10:32:12,564   
	//ASCII kodovi: Kada od ASCII koda za broj oduzmes '0', dobijes zapravo taj broj.
	//Algoritam objasni uzivo
	h = ((time[0] - '0') * 10) + (time[1] - '0');//char 'a'
	min = ((time[3] - '0') * 10) + (time[4] - '0');
	sec = ((time[6] - '0') * 10) + (time[7] - '0');
	msec = ((time[9] - '0') * 100) + ((time[10] - '0') * 10) + (time[11] - '0');
	Time* ret = new Time(h, min, sec, msec);
	return ret;

}

// proveri da nije slovo.

//00:00:00,000 --> 00:00:00,000
bool Time::isValid(std::string time) {
	bool valid = true;
	//za pocetno vreme
	if (time.length() != 29) return false; // Ako je duzina okej, samo treba da proverimo da li su na odredjenim mestima okej cifre,zarezi,dvotacke,crtice,etc.
	//proveri da li su minuti okej
	if (time[3] > '5')return false;
	//proveri da li su sekunde okej
	if (time[6] > '5') return false;
	//nema sta da se proveri za milisekunde, na sva tri mestu su dozvoljene sve cifre

	//za krajnje vreme
	if (time[20] > '5')return false;
	if (time[23] > '5')return false;

	//proverimo da li je crtica na sredini okej

	if (time[13] != '-' || time[14] != '-' || time[15] != '>')return false;

	//proverimo da li su razmaci na mestu
	if (time[12] != ' ')return false;
	if (time[16] != ' ')return false;

	//proverimo dvotacke i zareze
	if (time[2] != ':' || time[5] != ':' || time[8] != ',' || time[19] != ':' || time[22] != ':' || time[25] != ',') return false;

	//ako prodjemo sve ove if-ove, i ne udjemo ni u jedan, znaci da je time validan, pa cemo vratiti true
	return true;
}


Time* Time::msecToTime(int ms) {
	int seconds = 0;
	int minutes = 0;
	int hours = 0;
	int miliseconds = ms % 1000;
	if (ms / 1000 != 0) {
		seconds = (ms / 1000) % 60;
		minutes = (ms / (1000 * 60)) % 60;
		hours = ms / (1000 * 60 * 60);
	}

	Time* time = new Time(hours, minutes, seconds, miliseconds);
	return time;
}

int Time::timeToMsec(Time* time) {
	int milisec = time->msec + (time->sec * 1000) + (time->min * 60 * 1000) + (time->h * 60 * 60 * 1000);
	return milisec;
}

Time* Time::oduzimanjeVremena(Time* t1, Time* t2) {
	int msec1 = timeToMsec(t1);
	int msec2 = timeToMsec(t2);
	int msec = 0;
	if (msec1 > msec2) msec = msec1 - msec2;
	Time* time = msecToTime(msec);

	return time;
}
Time* Time::sabiranjeVremena(Time* t1, Time* t2) {
	int msec1 = timeToMsec(t1);
	int msec2 = timeToMsec(t2);
	int msec = msec1 + msec2;
	Time* time = msecToTime(msec);

	return time;
}

int Time::trajanje(Time* t1, Time* t2) {
	return timeToMsec(t2) - timeToMsec(t1);
}

bool Time::prviManji(Time* t1, Time* t2) { //pitamo da li je prvo vreme manje
	bool b = (timeToMsec(t1) < timeToMsec(t2)); // radimo tako sto vremena pretvorimo u milisekunde i pitamo da li je vrednost prvog broja manja od vrednosti drugog 
	return b; // vraca true ako je prvi manji, ili vraca false ako je drugi manji
}
#include "Time.h"
std::string Time::timeToStr() { //Konvertujemo objekat tipa Time u string
	//objekat Time npr izgleda ovako 01:21:05:050
	std::string ret = ""; //na pocetku string je prazan
	if (this->h / 10 == 0) ret += std::to_string(0); //Ako je broj h jednocifren, to zna;i da moramo da dodamo nulu na poetku stringa. ret="0"
	ret += std::to_string(h);// konvertujemo h u string i nalepimo ga na string. ret="01"
	ret += ":";//ret="01:"


	if (min / 10 == 0) ret += std::to_string(0); //isto kao za h, s tim da ovde, u nasem primeru, necemo nalepiti nula jer je broj dvocifren. ret="01:"
	ret += std::to_string(min); //isto kao za h. ret="01:21"
	ret += ":";// ret="01:21:"

	if (sec / 10 == 0) ret += std::to_string(0); //nalepljujemo nulu jer je broj jednocifren. ret="01:21:0"
	ret += std::to_string(sec); //nalepljujemo broj. ret="01:21:05"
	ret += ","; //ret = "01:21:05,"

	if (msec / 100 == 0)ret += std::to_string(0); //Nalepljujemo nulu jer je broj dvocifren. // ret = "01:21:05,0"
	if (msec / 10 == 0)ret += std::to_string(0);//Ovo pita da li je broj jednocifren, pa da nalepi jos jednu nulu. U nasem primeru nece nista uraditi. ret = "01:21:05,0"
	ret += std::to_string(msec); // ret = "01:21:05,050"
	return ret;//Uspesno smo konvertovali Time u string, jej

}

Time* Time::strToTime(std::string time) {
	int h, min, sec, msec;
	//ASCII kodovi: Kada od ASCII koda za broj oduzmes '0', dobijes zapravo taj broj.
	//Algoritam objasni uzivo
	h = ((time[0] - '0') * 10) + (time[1] - '0');
	min = ((time[3] - '0') * 10) + (time[4] - '0');
	sec = ((time[6] - '0') * 10) + (time[7] - '0');
	msec = ((time[9] - '0') * 100) + ((time[10] - '0') * 10) + (time[11] - '0');
	Time* ret = new Time(h, min, sec, msec);
	return ret;

}


//00:00:00,000 --> 00:00:00,000
bool Time::isValid(std::string time) {
	bool valid = true;
	//za pocetno vreme
	if (time.length() != 29) return false; // Ako je duzina okej, samo treba da proverimo da li je na odredjenim mestima, okej cifra.
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
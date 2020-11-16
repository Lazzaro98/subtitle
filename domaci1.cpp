// domaci1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Time.h"
#include "SubtitleEditor.h"
#include <sstream>
#include <fstream>
using namespace std;



int main()
{
	ifstream input("C:\\Users\\ml170233d\\source\\repos\\domaci1\\Debug\\input.txt");
	stringstream ss;
	ss << input.rdbuf(); input.close();
	string subtitles = ss.str();
	
	//cout << subtitles;
	int x; cin >> x;
	SubtitleEditor* SE = new SubtitleEditor();
	SE->loadSubtitle(subtitles);
	SE->addToSelection(1, 18);
	//SE->boldText(1, 6);

	


	cin >> x;
	SubtitleEditor::Elem* pom = SE->prvi;
	int brojac = 0;
	while (pom->next) {
		if (pom->next->tekst == "") {
			cout << "prazan je id"<<pom->next->id<<"\n";
			cout << "Tekst je" << pom->next->tekst << "\n";
			cout << "A tekst prethodnika je " << pom->tekst << "\n\n";
		}
		pom = pom->next;
	}
	cout << "brojac je" << brojac;
	
}


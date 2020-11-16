// DomPrviDeo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "Time.h"
#include "SubtitleEditor.h"
#include <sstream>
#include <fstream>
using namespace std;
int main()
{
    ifstream input("C:\\Users\\board\\Desktop\\javni_test - Copy\\input.txt");
    stringstream ss;
    ss << input.rdbuf(); input.close();
    string subtitles = ss.str();

    SubtitleEditor* SE = new SubtitleEditor();
    SE->loadSubtitle(subtitles);
    SE->addToSelection(1, 1000);
    SE->boldText(17,100);
    cout << SE->saveSubtitle();
    int x; cin >> x;
}

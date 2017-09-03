#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctype.h>
#include <string>
#include <sstream>
using namespace std;
ifstream fromFile;
ofstream toFile;

void write(string text) {
	toFile.open("dataFile.txt", ios::out | ios::app); // Open the database file.

	toFile << text << endl;

	toFile.close();

}
void addDialog() {
	string code, cCode, cName;

	cout << "Add dialog entered:" << endl;
	cout << "Type 'c' to add  city, 't' to add a team, or 'g' to add a game";
	cin >> code;
	if (code == "c") {
		cin >> cCode;
		cin >> cName;
		write("<city><" + cCode + ">" + "<" + cName + ">");
	}
	else if (code == "g") {

	}
	else if (code == "t") {

	}

}
void listDialog() {

}

int main() {


	string code,cCode,cName;
	//Read instructions to perform until an instruction to quit has been reached
	while (code != "q") {
		cout << "Please tell me what to do by typing one of the following codes:" << endl;
		cout << "'a' to add data" << endl;
		cout << "'l' to return a list of data" << endl;
		cout << "'r' for results of a specific team" << endl;
		cout << "'s' for standings of all the teams" << endl;
		cout << "'q' to quit" << endl;
		cin >> code;
			//For a code of "a" enter the add dialog
		if (code == "a") {
			addDialog();			
		}
		else if (code == "l") {
			listDialog();
		}
	}

	return 0;
}
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

int main() {


	string instruction;
	//Read instructions to perform on game listing
	while (instruction != "x") {
		cin >> instruction;
		write(instruction);

	}

	return 0;
}
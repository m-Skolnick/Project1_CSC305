#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctype.h>
#include <string>
#include <sstream>
using namespace std;
ifstream fromFile;
ifstream fileSearcher;
ofstream toFile;
	//Create record structure to store team record
struct record {
	string tName = "blank";
	int wins = 0, losses=0, PF=0, PA=0;
};

void writeToFile(string text) {
	toFile.open("dataFile.txt", ios::out | ios::app); //Open the database file.
	toFile << text << endl; //Write text into file
	toFile.close(); //Close database file
}
void addDialog() {
	char code2 = 'q';
	string cCode, cName, tName, vtName, htName;
	int vtScore, htScore;
		//Prompt user
	cout << "Add dialog entered:" << endl;
	cout << "Type instruction:" << endl;
	cout << "'c' to add  city" << endl;
	cout << "'t' to add a team" << endl;
	cout << "'g' to add a game" << endl;
	cout << "'q' to return to previous dialog" << endl << ">>> ";
	cin >> code2; //Get next code
	switch (code2) { //Enter the correct dialog based on the character entered
	case 'c':
		cout << "Enter the city code" << endl << ">>> ";
		cin >> cCode;
		cout << "Enter the city name" << endl << ">>> ";
		cin.ignore();
		getline(cin, cName);
		writeToFile("<city>");
		writeToFile(cCode);
		writeToFile(cName);
		cout <<"Successfully saved City code: "+cCode+", Name: "+cName<<endl<<endl;
		break;
	case 't':
		cout << "Enter the city code for your new team" << endl << ">>> ";
		cin >> cCode;
		cout << "Enter the new team name" << endl << ">>> ";
		cin >> tName;
		writeToFile("<team>");
		writeToFile(cCode);
		writeToFile(tName);
		cout << "Successfully saved Team Name: " + tName + ", City: " + cCode << endl << endl;
		break;
	case 'g': 
		cout << "Enter the visiting team name" << endl << ">>> ";
		cin >> vtName;
		cout << "Enter the visiting team score" << endl << ">>> ";
		cin >> vtScore;
		cout << "Enter the home team name" << endl << ">>> ";
		cin >> htName;
		cout << "Enter the home team score" << endl << ">>> ";
		cin >> htScore;
			//Add the game to the data file
		writeToFile("<game>");
		writeToFile(vtName);
		writeToFile(to_string(vtScore));
		writeToFile(htName);
		writeToFile(to_string(htScore));
			//Alert user that game was successfully saved
		cout << "Successfully saved game:" << endl;
		cout << vtName + ": " + to_string(vtScore)+" "+htName+": "+to_string(htScore) << endl << endl;
		break;
	}
}
void getCities() {
	string line ="start", cName, cCode;
	fromFile.open("dataFile.txt");
	cout << "= City Name ===== Code" << endl;
	while (getline(fromFile, line)) {
		if (line == "<city>") {
			getline(fromFile, cCode);
			getline(fromFile, cName);
			cout << " " << left << setw(17) << cName << cCode << endl;
		}
	}
	cout << endl; // Add a line after printing data
	fromFile.close(); //Close the data file
}
string findTeam(string findCode) {
	fileSearcher.open("dataFile.txt"); //Open data file again to start at top
	string line = "blank",cName = "(City not found)",cCode;
	while (getline(fileSearcher, line)) { //Search through every line of the data file
		if (line == "<city>") { //Check each city listing
			getline(fileSearcher, cCode);
			if (cCode == findCode) { //Return City name if city code matches the one searched
				getline(fileSearcher, cName);
			}
		}
	}
	fileSearcher.close(); //Close the data file
	return cName;
}
void getTeams() {
	string line = "blank", cName, cCode, tName;
	fromFile.open("dataFile.txt");
	cout << "= Team Name ===== City" << endl;
	while(getline(fromFile, line)){
		if (line == "<team>") {
			getline(fromFile, cCode);
			getline(fromFile, tName);
			cName = findTeam(cCode); //Find the city name corresponding with the city code
			cout << " " << left << setw(17) << tName << cName << endl;
		}
	}
	cout << endl; // Add a line after printing data
	fromFile.close(); //Close the data file
}
void printGames() {
	string line = "start", vtName, htName,vtScore,htScore;
	fromFile.open("dataFile.txt");
	cout << "= Visiting Team = Score == Home Team ==== Score" << endl;
	while (getline(fromFile, line)) {
		if (line == "<game>") {
			getline(fromFile, vtName);
			getline(fromFile, vtScore);
			getline(fromFile, htName);
			getline(fromFile, htScore);			
			cout << "  " << left << setw(18) << vtName << setw(7) << vtScore 
				<< setw(17) << htName << htScore << endl;
		}
	}
	cout << endl; // Add a line after printing data
	fromFile.close(); //Close the data file
}
void listDialog() {
	char code2 = 'q';
	string cCode, cName, tName, vtName, htName;
	//int vtScore, htScore;
	//Prompt user
	cout << "List dialog entered:" << endl;
	cout << "Type instruction:" << endl;
	cout << "'c' to list cities" << endl;
	cout << "'t' to list all teams" << endl;
	cout << "'g' to list all games" << endl;
	cout << "'q' to return to previous dialog" << endl << ">>> ";
	cin >> code2; //Get next code
	switch (code2) { //Enter the correct dialog based on the character entered
	case 'c': 
		getCities();
		break;
	case 't':
		getTeams();
		break;
	case 'g': 
		printGames();
		break;
	}
}
record getRecord(string searchName) {
	record tRecord;
	tRecord.tName = searchName;
	fileSearcher.open("dataFile.txt");
	string line = "start", vtName, htName, vtScoreStr, htScoreStr;
	int vtScore=0, htScore=0;
	int wins = 0;
	while (getline(fileSearcher, line)) {
		if (line == "<game>") {
			stringstream ss;
			getline(fileSearcher, vtName);
			getline(fileSearcher, vtScoreStr);
			getline(fileSearcher, htName);
			getline(fileSearcher, htScoreStr);
				//Convert string scores to integers
			ss.clear();
			ss << vtScoreStr;
			ss >> vtScore;
			ss.clear();
			ss << htScoreStr;
			ss >> htScore;
			if (vtName == searchName) { //If visiting team is the searched for team, count scores and wins
				tRecord.PF += vtScore;
				tRecord.PA += htScore;
				if (vtScore > htScore) {
					tRecord.wins++;
				}
				else if (htScore > vtScore) {
					tRecord.losses++;
				}
			}
			else if (htName == searchName) { //If visiting team is the searched for team, count scores and wins
				tRecord.PF += htScore;
				tRecord.PA += vtScore;
				if (htScore > vtScore) {
					tRecord.wins++;
				}
				else if (vtScore > htScore) {
					tRecord.losses++;
				}
			}
		}
	}
	fileSearcher.close(); //Close the data file
	return tRecord;
}
// ****** NEEDS WORK *********************
void buildRecordList() {
	fileSearcher.open("dataFile.txt");
	string line = "blank";
	int gameCount = 0;
	while (getline(fileSearcher, line)) {
		if (line == "<game>") {
			gameCount++;
		}
	}
	record* recordList = NULL;
	//Hit a dead end here........ 
}
void recordDialog() {
	string tName = "blank";
	int wins=0, losses=0, PF=0, PA=0;
	cout << "List dialog entered:" << endl;
	cout << "Please enter the team name which you would like the record for:" << endl << ">>> ";
	cin >> tName;
	record tRecord = getRecord(tName);

	cout << "=Team=============W===L===PF===PA" << endl;
	cout << " " << left << setw(17) << tName << setw(4) << tRecord.wins << setw(4) << tRecord.losses
		<< setw(5) << tRecord.PF << setw(5) << tRecord.PA << endl;
	
}
void standingsDialog() {

}

int main() {
	char code = 'q';
	//Read instructions to perform until an instruction to quit has been reached
	do {
		cout << "type 'i' to start a new instruction" << endl;
		cin >> code;
		cout << "New instruction started, please type one of the following codes:" << endl;
		cout << "'a' to add data" << endl;
		cout << "'l' to return a list of data" << endl;
		cout << "'r' for results of a specific team" << endl;
		cout << "'s' for standings of all the teams" << endl;
		cout << "'q' to quit" << endl << ">>> ";
		cin >> code; //Get the first code
		switch (code) { //Enter the correct dialog based on the character entered
		case 'a': addDialog(); break;
		case 'l': listDialog(); break;
		case 'r': recordDialog(); break;
		case 's': standingsDialog(); break;
		}
	}
	while (code != 'q');
	return 0;
}
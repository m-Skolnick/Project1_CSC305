#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctype.h>
#include <string>
#include <sstream>
using namespace std;
	//Create team structure to store team record
struct team {
	string tName = "blank", cCode="blank", cName = "blank"; //Want to change this to NULL
	int wins = 0, losses = 0, PF = 0, PA = 0;
	float winningPercentage = 0, pointDifferential=0;
};
ifstream fromFile, fileSearcher;
ofstream toFile;
team* teamList = NULL;
int gameCount=0,teamCount=0;

void writeToFile(string text) {
	toFile.open("dataFile.txt", ios::out | ios::app); //Open the database file.
	toFile << text << endl; //Write text into file
	toFile.close(); //Close database file
}
void printCities() {
	string line ="start", cName, cCode;
	fromFile.open("dataFile.txt");
	cout << "= City Name ======== Code" << endl;
	while (getline(fromFile, line)) {
		if (line == "<city>") {
			getline(fromFile, cCode);
			getline(fromFile, cName);
			cout << " " << left << setw(20) << cName << cCode << endl;
		}
	}
	cout << endl; // Add a line after printing data
	fromFile.close(); //Close the data file
}
void printTeams() {
	cout << "= Team Name ========= City Name" << endl;
	for (int i = 0; i<teamCount; i++) {
		cout << left << " " << setw(20) << teamList[i].tName << teamList[i].cName << endl;		
	}
}
void printGames() {
	string line = "start", vtName, htName, vtScore, htScore;
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
string findCityNameInFile(string findCode) {
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
int getTeamIndexFromTeamList(string tName) {
	for (int i = 0; i<teamCount; i++) { //Find the index for the searched for team
		if (tName == teamList[i].tName) {
			return i; //Return the index when the team is found
		}
	}
}
void addStatsToTeamList() {
	float tempWins = 0, tempLosses = 0;
	for (int i = 0; i<teamCount; i++) {
		tempWins = teamList[i].wins;
		tempLosses = teamList[i].losses;
		teamList[i].winningPercentage = tempWins /(tempWins + tempLosses);
		teamList[i].pointDifferential = teamList[i].PF - teamList[i].PA;
	}
}
void addCitiesToTeamList() {
	for (int i = 0; i<teamCount; i++) {
		teamList[i].cName = findCityNameInFile(teamList[i].cCode);
	}
}
void addScoresToTeamList() {
	fileSearcher.open("dataFile.txt");//Start a new search of the file from the top	
	string line = "blank",vtName ="0",vtScoreStr ="0",htName="blank",htScoreStr="blank";
	int i=0, vtScore=0,htScore=0,htIndex=0,vtIndex=0;
	while (getline(fileSearcher, line)) {
		if (line == "<game>") {
				//Get each of the values for every game
			getline(fileSearcher, vtName);
			getline(fileSearcher, vtScoreStr);
			getline(fileSearcher, htName);
			getline(fileSearcher, htScoreStr);
				//Convert string scores to integers
			stringstream ss;
			ss.clear();
			ss << vtScoreStr;
			ss >> vtScore;
			ss.clear();
			ss << htScoreStr;
			ss >> htScore;
				//Find the index for the team in the array of teams
			htIndex=getTeamIndexFromTeamList(htName);
			vtIndex=getTeamIndexFromTeamList(vtName);
				//Add the info to the records at the indexes found
			teamList[htIndex].PF += htScore;
			teamList[htIndex].PA += vtScore;
			teamList[vtIndex].PF += vtScore;
			teamList[vtIndex].PA += htScore;
				//If the home team won, add win count to the home team and loss count to visitor
			if(htScore>vtScore){
				teamList[htIndex].wins++;
				teamList[vtIndex].losses++;
			}
				//If the visiting team won, add win count to the visiting team and loss count to home
			else if(htScore<vtScore){
				teamList[vtIndex].wins++;
				teamList[htIndex].losses++;
			}
		}
	}
	fileSearcher.close();
}
void buildTeamList() {
	fileSearcher.open("dataFile.txt");
	string line = "blank"; //Change this to NULL if possible *************************************************
	teamCount = 0;
	//Count the number of teams
	while (getline(fileSearcher, line)) {
		if (line == "<team>") {
			teamCount++;
		}
	}
	fileSearcher.close(); //Close the open file 
	fileSearcher.open("dataFile.txt"); //Start a new search of the file from the top
									   //Create an array of teams with the max number of teams
	teamList = new team[teamCount];
	int i = 0;
	while (getline(fileSearcher, line)) {
		if (line == "<team>") { //For each team, add it to the team list
			getline(fileSearcher, teamList[i].cCode);
			getline(fileSearcher, teamList[i].tName);
			i++;
		}
	}
	fileSearcher.close(); //Close the file
	addCitiesToTeamList(); //Add all of the city names which match codes to the team list
	addScoresToTeamList(); //Add all of the scores to the list of teams
	addStatsToTeamList(); //Add each of the winning percentages to the team list
}
void addDialog() {
	char code2 = 'q';
	string cCode, cName, tName, vtName, htName;
	int vtScore, htScore;
	cin >> code2; //Get next code for what will be added
	switch (code2) { //Enter the correct dialog based on the character entered
	case 'c':
		cin >> cCode >> ws; //Get the city code
		getline(cin, cName); //Get the city name
			//Write the new city/name to the file
		writeToFile("<city>");
		writeToFile(cCode);
		writeToFile(cName);
			//Alert user of success
		cout <<"Successfully saved City code: "+cCode+", Name: "+cName<<endl<<endl;
		break;
	case 't':
		cin >> cCode; //Get the city code for the new team
		cin >> tName; //Get the team name for the new team
			//Write the new team/city code to the file
		writeToFile("<team>");
		writeToFile(cCode);
		writeToFile(tName);
			//Alert user of success
		cout << "Successfully saved Team Name: " + tName + ", City: " + cCode << endl << endl;
		break;
	case 'g': 
			//Get the visiting team name,team score,home team name, and home team score
		cin >> vtName >> vtScore >> htName >> htScore; 
			//Add the game to the data file
		writeToFile("<game>");
		writeToFile(vtName);
		writeToFile(to_string(vtScore));
		writeToFile(htName);
		writeToFile(to_string(htScore));
			//Alert user of success
		cout << "Successfully saved game:" << " ";
		cout << vtName + ": " + to_string(vtScore)+" "+htName+": "+to_string(htScore) << endl << endl;
		break;
	}
}
void listDialog() {
	char code2 = 'q';
	string cCode, cName, tName, vtName, htName;
	cin >> code2; //Get next code
	switch (code2) { //Enter the correct dialog based on the character entered
	case 'c': 
		printCities();
		break;
	case 't':
		printTeams();
		break;
	case 'g': 
		printGames();
		break;
	}
}
void printRecord(int teamIndex) {
	cout << " " << left << setw(17) << teamList[teamIndex].tName << setw(4) << teamList[teamIndex].wins << setw(4) << teamList[teamIndex].losses
		<< setw(5) << teamList[teamIndex].PF << setw(5) << teamList[teamIndex].PA << endl;
}
void recordDialog() {
	string tName = "blank";
	int teamIndex = 0;
	cin >> tName; //Get the name of the team to show record for
	teamIndex = getTeamIndexFromTeamList(tName); //Find the index in the team list
	cout << "=Team=============W===L===PF===PA" << endl; //Print data label
	printRecord(teamIndex); //Print team record
	cout << endl; //Add another space after printing team record
}
void standingsDialog() {
	int indexToPrint = 0;
	float lastWP = 0, lastPD = 0;
	cout << "=Team=============W===L===PF===PA" << endl; //Print data label
	for (int i = 0; i<teamCount; i++) {
		if (teamList[i].winningPercentage > lastWP) {
			indexToPrint = i;
		}
		if (teamList[i].winningPercentage = lastWP) {
			if(teamList[i])
		}
	}
	printRecord(indexToPrint);
}

int main() {
	char code = 'q'; //Start with q, so if nothing is added, code quits
		//Read instructions to perform until an instruction to quit has been reached
	do {
		buildTeamList(); //Build a list of teams already in the file
		cout << ">>>";
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

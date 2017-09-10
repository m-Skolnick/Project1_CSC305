#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctype.h>
#include <string>
#include <sstream>
using namespace std;
	//Create team structure to store team record
struct team {
	string tName = "blank", cCode="blank", cName = "blank";
	int wins = 0, losses = 0, PF = 0, PA = 0;
	float WP = 0, PD=0;
};
	//Declare public variables
ifstream fromFile;
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
	fromFile.open("dataFile.txt"); //Open the data file
		//Print header
	cout << "= City Name ======== Code" << endl;
		//Print each of the cities which were entered into the database
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
		//Print data header
	cout << "= Team Name ========= City Name" << endl;
	for (int i = 0; i<teamCount; i++) { //Print each of the teams in the team list
		cout << left << " " << setw(20) << teamList[i].tName << teamList[i].cName << endl;		
	}
	cout << endl; //Add another space after printing all teams
}
void printGames() {
	string line = "start", vtName, htName, vtScore, htScore;
	fromFile.open("dataFile.txt"); //Open the data file
	cout << "= Visiting Team = Score == Home Team ==== Score" << endl; //Print data header
	while (getline(fromFile, line)) {
		if (line == "<game>") { //Print each of the games in the data file
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
	fromFile.open("dataFile.txt"); //Open data file again to start at top
	string line = "blank",cName = "(City not found)",cCode;
	while (getline(fromFile, line)) { //Search through every line of the data file
		if (line == "<city>") { //Check each city listing
			getline(fromFile, cCode);
			if (cCode == findCode) { //Return City name if city code matches the one searched
				getline(fromFile, cName);
			}
		}
	}
	fromFile.close(); //Close the data file
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
	for (int i = 0; i<teamCount; i++){
			//Convert each of the integers to floating point numbers for later floating point division
		tempWins = teamList[i].wins;
		tempLosses = teamList[i].losses;
			//Calculate winning percentage and point differential
		teamList[i].WP = tempWins /(tempWins + tempLosses);
		teamList[i].PD = teamList[i].PF - teamList[i].PA;
	}
}
void addCitiesToTeamList() {
		//For the city code for each team in the team list, search the data file for a city name
	for (int i = 0; i<teamCount; i++) {
		teamList[i].cName = findCityNameInFile(teamList[i].cCode);
	}
}
void addScoresToTeamList() {
		//This function works because it was assumed that each team (visiting and home) 
			//were already entered as valid teams before the game was entered. 
			//If a team was not entered in the database, the function may crash.
	fromFile.open("dataFile.txt");//Start a new search of the file
	string line = "blank",vtName ="0",vtScoreStr ="0",htName="blank",htScoreStr="blank";
	int i=0, vtScore=0,htScore=0,htIndex=0,vtIndex=0;
	while (getline(fromFile, line)) {
		if (line == "<game>") {
				//Get each of the values for every game
			getline(fromFile, vtName);
			getline(fromFile, vtScoreStr);
			getline(fromFile, htName);
			getline(fromFile, htScoreStr);
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
	fromFile.close();
}
void buildTeamList() {
	fromFile.open("dataFile.txt");
	string line = "blank";
	teamCount = 0;
		//Count the number of teams
	while (getline(fromFile, line)) {
		if (line == "<team>") {
			teamCount++;
		}
	}
	fromFile.close(); //Close the open file 
	fromFile.open("dataFile.txt"); //Start a new search of the file
	teamList = new team[teamCount];//Create an array of teams with the max number of teams
	int i = 0;
	while (getline(fromFile, line)) {
		if (line == "<team>") { //For each team, add it to the team list
			getline(fromFile, teamList[i].cCode);
			getline(fromFile, teamList[i].tName);
			i++;
		}
	}
	fromFile.close(); //Close the file
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
void printTeamRecord(int teamIndex) {
		//Print the team record at the specified index in the team list array
	cout << " " << left << setw(17) << teamList[teamIndex].tName << setw(4) << teamList[teamIndex].wins << setw(4) << teamList[teamIndex].losses
		<< setw(5) << teamList[teamIndex].PF << setw(5) << teamList[teamIndex].PA << endl;
}
void recordDialog() {
	string tName = "blank";
	int teamIndex = 0;
	cin >> tName; //Get the name of the team to show record for
	teamIndex = getTeamIndexFromTeamList(tName); //Find the index in the team list
	cout << "=Team=============W===L===PF===PA" << endl; //Print data label
	printTeamRecord(teamIndex); //Print team record
	cout << endl; //Add another space after printing team record
}
void sortByStandings() {
	bool sorted = false;
	team tempTeam = team();
	while (!sorted) {
		sorted = true;
		for (int i = 0; i < teamCount-1; i++) {
				//If the current WP is lower, swap the teams
			if (teamList[i].WP < teamList[i + 1].WP) {
				tempTeam = teamList[i + 1];
				teamList[i + 1] = teamList[i];
				teamList[i] = tempTeam;
				sorted = false;
			}
				//If the current WP is the same as the next, check the PD 
			else if (teamList[i].WP == teamList[i + 1].WP) {
					//If the current PD is lower, swap the teams
				if (teamList[i].PD < teamList[i + 1].PD) {
					tempTeam = teamList[i + 1];
					teamList[i + 1] = teamList[i];
					teamList[i] = tempTeam;
					sorted = false;
				}
					//If the current PD is the same as the next, sort alphabetically
				else if (teamList[i].PD == teamList[i + 1].PD) {
					if (teamList[i].tName > teamList[i + 1].tName) {
						tempTeam = teamList[i + 1];
						teamList[i + 1] = teamList[i];
						teamList[i] = tempTeam;
						sorted = false;
					}
				}
			}
		}
	}
}
void standingsDialog() {
	sortByStandings(); //Sort all of the teams by their standings
	cout << "=Team=============W===L===PF===PA" << endl; //Print data label
	for (int i = 0; i<teamCount; i++) {
		printTeamRecord(i); //Print each team
	}	
	cout << endl; //Add another space after printing all the teams
}
int main() {
	char code = 'q'; //Start with q, so if nothing is added, code quits
		//Read instructions to perform until an instruction to quit has been reached
	do {		
		cout << ">>>"; //Prompt the user for input
		cin >> code; //Get the first code
		buildTeamList(); //Build a list of teams already in the file
		switch (code) { //Enter the correct dialog based on the character entered
		case 'a': addDialog(); break;
		case 'l': listDialog(); break;
		case 'r': recordDialog(); break;
		case 's': standingsDialog(); break;
		}
	}
	while (code != 'q'); //Keep reading instructions until a code of "q" is reached
	return 0;
}

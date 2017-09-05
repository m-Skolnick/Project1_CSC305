#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctype.h>
#include <string>
#include <sstream>
using namespace std;

	//Create record structure to store team record
struct record {
	string tName = "blank";
	int wins = 0, losses=0, PF=0, PA=0;
};
	//Create record structure to store team record
struct team {
	string tName = "blank"; //Want to change this to NULL
	int wins = 0, losses=0, PF=0, PA=0;
};
struct game {
	string vtName = "blank",htName="blank";
	int vtScore = 0, htScore = 0;
};
ifstream fromFile, fileSearcher;
ofstream toFile;
record* recordList = NULL;
game* gameList = NULL;
team* teamList = NULL;
int gameCount,teamCount;

void writeToFile(string text) {
	toFile.open("dataFile.txt", ios::out | ios::app); //Open the database file.
	toFile << text << endl; //Write text into file
	toFile.close(); //Close database file
}
void addDialog() {
	char code2 = 'q';
	string cCode, cName, tName, vtName, htName;
	int vtScore, htScore;
		//Get the next code
	cin >> code2; //Get next code for what will be added
	switch (code2) { //Enter the correct dialog based on the character entered
	case 'c':
		cin >> cCode; //Get the city code
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
		cin >> vtName; //Get the visiting team name
		cin >> vtScore; //Get the visiting team score
		cin >> htName; //Get the home team name
		cin >> htScore; //Get the home team score
			//Add the game to the data file
		writeToFile("<game>");
		writeToFile(vtName);
		writeToFile(to_string(vtScore));
		writeToFile(htName);
		writeToFile(to_string(htScore));
			//Alert user of success
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
void buildTeamList() {
	fileSearcher.open("dataFile.txt");	
	string line = "blank"; //Change this to NULL if possible *************************************************
	teamCount = 0;
		//Count the number of teams played
	while (getline(fileSearcher, line)) {
		if (line == "<team>") {
			teamCount++;
		}
	}		
	fileSearcher.close(); //Close the open file 
	fileSearcher.open("dataFile.txt"); //Start a new search of the file from the top
		//Create an array of teams with the max number of teams
	teamList = new team[teamCount];
	int i=0;
	while (getline(fileSearcher, line)) { 
		if (line == "<team>") { //For each team, add it to the team list
			getline(fileSearcher, teamList[i].cCode);
			getline(fileSearcher, teamList[i].tName);
			i++;
		}
	}
	fileSearcher.close(); //Close the file
}
void addScoresToTeamList() {
	fileSearcher.open("dataFile.txt");//Start a new search of the file from the top	
	string line = "blank"; //Change this to NULL if possible *************************************************	
	int i=0;
	while (getline(fileSearcher, line)) {
		if (line == "<game>") {
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
			htIndex=getTeamIndex(htName);
			vtIndex=getTeamIndex(vtName);
			
			
			//Needs work!!!!!!!!
			teamList[htIndex].vtName = vtName;
			gameList[gameCount].htName = htName;
			gameList[gameCount].vtScore = vtScore;
			gameList[gameCount].htScore = htScore;
			gameCount++;
		}
	}
	fileSearcher.close();
}
void buildGameList() {
	fileSearcher.open("dataFile.txt");	
	string line = "blank"; //Change this to NULL if possible *************************************************
	gameCount = 0;
		//Count the number of games played
	while (getline(fileSearcher, line)) {
		if (line == "<game>") {
			gameCount++;
		}
	}
	fileSearcher.close();
	//Create an array of records with the max number of teams..ie..one per game
	gameList = new game[gameCount];
	string vtName, htName, vtScoreStr, htScoreStr;
	int vtScore = 0, htScore = 0;

	fileSearcher.open("dataFile.txt");
	line = "blank";
	gameCount = 0;
	while (getline(fileSearcher, line)) {
		if (line == "<game>") {
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
			gameList[gameCount].vtName = vtName;
			gameList[gameCount].htName = htName;
			gameList[gameCount].vtScore = vtScore;
			gameList[gameCount].htScore = htScore;
			gameCount++;
		}
	}
	fileSearcher.close();
}
void buildRecordList() {

	recordList = new record[gameCount * 2];
	string searchName = "blank";
	int count = 0;
	int gamecount = sizeof(gameList);
	while (count < sizeof(gameList)) {
		if (gameList[count].vtName == searchName) { //If visiting team is the searched for team, count scores and wins
			recordList[count].PF += gameList[count].vtScore;
			recordList[count].PA += gameList[count].htScore;
			if (gameList[count].vtScore > gameList[count].htScore) {
				recordList[count].wins++;
			}
			else if (gameList[count].htScore > gameList[count].vtScore) {
				recordList[count].losses++;
			}
		}
		else if (gameList[count].htName == searchName) { //If visiting team is the searched for team, count scores and wins
			recordList[count].PF += gameList[count].htScore;
			recordList[count].PA += gameList[count].vtScore;
			if (gameList[count].htScore > gameList[count].vtScore) {
				recordList[count].wins++;
			}
			else if (gameList[count].vtScore > gameList[count].htScore) {
				recordList[count].losses++;
			}
		}
		count++;
	}
}
void recordDialog() {
	//buildGameList();
	//buildRecordList();
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

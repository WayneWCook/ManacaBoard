/* Board.cpp
 * Wayne Cook
 * 22 September 2020
 * Create the start of the Board file for the Mancala game.
 * Modified: Wayne Cook
 *		10 October 2020
 *		Adding the movescapability to the game.
 * Modified: Wayne Cook
 *		11 October 2020
 *		Adding user statistics and opponent stone capture to the game.
 * Modified: Wayne Cook
 *		12 October 2020
 *		Adding file I/O to track the users performance over time
 * Modified: 29 October 2020
 *		Gather all Player variables to a single structure
 *      and adding ANSI COLORS and BACKGROUNDS
 * Modified: 1 november 2020
 *		Because of the use of the Player structure, code size can be drastically reduced.
 *		printing the two player sides and the index/value loops are all reduced to one loop.
 */

 //Libraries to include
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <random>										// Random number generator.
#include <ctime>										// Add a way to seed random number generator
using namespace std;

// Include the Player Structure
#include "Player.h";

// List out the prototypes

/* void playerMove(int &player) - from rendition 3
 * Output: Modified player
 * Input: int &player (1 or 2)
 */
void playerMove(int&);

/* int initializePlayers() - from rendition 4
 * Output: Returns the number of the player whose starts the game.
 * Input: None
 * Purpose: Initialize the names of the players and their statistics.
 */
int initializePlayers();

/* int findWinner() - from rendition 4
 * Output: Returns the number of the player who won the game.
 * Input: None
 * Purpose: Determine the winner and update the statistics.
 */
int findWinner();

/* int checkEmptySide() - from rendition 3
 * Output: Returns the number of the player whose side is empty
 * Input: None
 */
int checkEmptySide();

/* int storeStatistics() - from rendition 5
 * Output: None
 * Input: None
 * Purpose: Write all the statistics to a file.
 */
void storeStatistics();

// Global variables for board - "const int" neabs the values cannot be changed in the program.
const int resetBoard[] = { 4,4,4,4,4,4,0,4,4,4,4,4,4,0 };	// Default values, used for each new game
int board[14];												// Where stone values kept during the game.
/* global vaiables for storing the player information. Each first element of each array is not used
 * so that I can just use the player's number for indexing into each array without doing subtraction
 */
Player players[3];									// Move all player related variables to one place
string fileNameBase = "mancala";					// Base name of file for info storage.


/* bool solidLine(int, bool) - from rendition 1
 *   returns a boolean on success of the function
 *   Input:
 *     int length - How long do I want the line.
 *     bool newLine - whether I want a new line or not.
 */
bool solidLine(int length, bool newLine) {
	bool retVal = true;
	for (int i = 0; i < length; i++) cout << ANSI_COLOR_RESET "*";
	if (newLine) cout << endl;
	return retVal;
}

/* bool dottedLine(int, bool) - from rendition 1
 *   returns a boolean on success of the function
 *   Input:
 *     int length - How many dots do I want the line.
 *     bool newLine - whether I want a new line or not.
 */
bool dottedLine(int length, bool newLine) {
	bool retVal = true;
	for (int i = 0; i < length; i++) cout << "*    ";
	if (newLine) cout << ANSI_COLOR_WHITE "*" << endl;
	return retVal;
}

/* void printPlayerLines(int player) - from rendition for using Player struct
 * Output: None
 * Input: player number
 * Purpose: Write the pit number and how many stones are in each pit
 *     for each player. Higlight Mancala with the most/least/same stones.
 */
void printPlayerLines(int player) {
	int oppPlayer = player == 1 ? 2 : 1,		// Set the opposing player number
		plMncl = players[player].mancala, // Identify Player's Mancala
		opMncl = players[oppPlayer].mancala,	// Opponents Mancala
		value,									// Storage for value to be printed.
		startVal = player == 1 ? players[1].lowPit : players[2].mancala,	// Start of loop
		endVal = player == 1 ? players[1].mancala : players[2].lowPit;		// End of loop
	for (int y = 0; y < 2; y++) {				// Go through the loop twice
		if (player == 1) dottedLine(1, false);
		for (int i = startVal; player == 1 ? i <= endVal : i >= endVal;
			player == 1 ? i++ : i--) {
			cout << ANSI_COLOR_WHITE "*";
			cout << ANSI_COLOR_BRIGHT;
			if (i == plMncl) {
				if (board[plMncl] > board[opMncl]) cout << ANSI_COLOR_BLACK
					ANSI_BACKGROUND_CYAN;
				else if (board[plMncl] < board[opMncl]) cout << ANSI_BACKGROUND_RED;
				else cout << ANSI_COLOR_BLACK ANSI_BACKGROUND_YELLOW;
			}
			else if (y == 0) cout << ANSI_COLOR_WHITE;
			else cout << ANSI_COLOR_YELLOW;
			value = y == 0 ? i : board[i];
			cout << setw(3) << value << ' ';
			cout << ANSI_COLOR_RESET;
		}
		if (player == 1) cout << ANSI_COLOR_WHITE "*" << endl;
		else dottedLine(1, true);
	}
}

/* void printBoard() - from rendition 1 & 2
 * Input: None
 * Output: None
 * Purpose: Print the board.
 */
void printBoard() {
	// Player 2 side of board
	solidLine(41, true);
	dottedLine(8, true);
	printPlayerLines(2);
	dottedLine(8, true);
	// Middle line
	dottedLine(1, false);
	solidLine(30, false);
	dottedLine(1, true);
	// Player 1 side of Board
	dottedLine(8, true);
	printPlayerLines(1);
	dottedLine(8, true);
	solidLine(41, true);
}

// The main program start - from rendition 3
int main() {
	string input;									// Place where to put the input for continuing.
	srand((int)time(0));							// Sets up a seed, time needs to be cast to int.
	int startingPlayer,								// Place to keep who  started so that it can toggle
		player = initializePlayers();				// Stores current player toggles between 1 and 2
	players[1].mancala = 6;							// Set up pit locations for Player 1
	players[1].lowPit = 0;
	players[1].highPit = 5;
	players[2].mancala = 13;							// Set up pit locations for Player 2
	players[2].lowPit = 7;
	players[2].highPit = 12;
	do {											// Loop until done playing
		startingPlayer = player;					// Save for later use.
		// Set up the starting board
		for (int i = 0; i < 14; i++) board[i] = resetBoard[i];
		// Increment the games played for each player.
		players[1].games++;
		players[2].games++;
		
		// Now loop until one side of the board is cleared.
		while (player != 0) playerMove(player);
		// This is where the win and loss is determined
		findWinner();
		player = startingPlayer == 1 ? 2 : 1;			// Toggle who starts the next game.
		cout << "Do you want to play another game? (y/n): ";
		cin >> input;
	} while (toupper(input[0]) == 'Y');
	// Store the current statistics in a file
	storeStatistics();
	return 0;
}

/* void printPlayerStatistics()
 * Output: None
 * Input: None
 * Purpose: Print the statistics for the current two players
 */
void printPlayerStatistics() {
	cout << "Name\tGames\tWins\tLosses\tTies\tWin %" << endl;
	for (int i = 1; i <= 2; i++) {
		int percent;
		cout << players[i].name << "\t" << players[i].games << "\t" << players[i].wins << "\t"
			<< players[i].losses << "\t" << players[i].ties << "\t";
		if (players[i].games > 0) percent = 100 * players[i].wins / players[i].games;
		else percent = 0;
		cout <<  percent << "%" << endl;
	}
}
/* int initializePlayers() - from rendition 4
 * Output: Returns the number of the player whose starts the game.
 * Input: None
 * Purpose: Initialize the names of the players and their statistics.
 */
int initializePlayers() {
	int retVal = 1,
		numberChoice = -1,
		i;													// Index used for loading values
	string input, name2;
	int games2, wins2, losses2, ties2;
	ifstream inFile;
	ofstream outFile;
	for (i = 1; i <= 2; i++) {
		cout << "What is the name of player " << i << "? ";
		cin >> players[i].name;
		players[i].games = 0;
		players[i].wins = 0;
		players[i].losses = 0;
		players[i].ties = 0;
	}
	// Check for information already in the file for each player, then use the saved data
	inFile.open(fileNameBase + ".txt");				// Place where info from previous games stored
	if (inFile) {
		outFile.open(fileNameBase + ".bak");		// Store all information not used this game
		while (inFile >> name2 >> games2 >> wins2 >> losses2 >> ties2) {
			bool found = false;
			for (i = 1; i <= 2; i++) {
				if (players[i].name == name2) {
					players[i].games = games2;
					players[i].wins = wins2;
					players[i].losses = losses2;
					players[i].ties = ties2;
					found = true;
				}
			}
			if (!found) {
				outFile << name2 << " " << games2 << " " << wins2 << " " << losses2 << " "
					<< ties2 << endl;
			}

		}
		outFile.close();
	}
	inFile.close();
	// Print what the current users statistics are
	printPlayerStatistics();

	// Loop until a proper choice is made
	do {
		cout << "Which player do you want to start the game?\n1: " << players[1].name << "\n2: "
			<< players[2].name << "\n3: Flip a coin\nChoose: ";
		cin >> input;
		try {
			numberChoice = stoi(input);
		}
		catch (exception e) {
			cout << "You must enter a number.";
			numberChoice = -1;
		}
	} while ((numberChoice < 1) || (numberChoice > 3));
	// Now set the starting player.
	switch (numberChoice) {
	case 1:
		retVal = 1;
		break;
	case 2:
		retVal = 2;
		break;
	case 3:
		retVal = (rand() % 2) + 1;
	}
	return retVal;
}

/* void lastStonePlacement(int &player, int pitNum) - from rendition 4
 * Output: Modified player
 * Input:
		int &player (1 or 2)
		int pitNum - Number where last stone was dropped
 * Prupose: Check where last stone is dropped and determine next player and
 *		opponent stones capture. If a player lands on an empty pit on their own side,
 *		all the stones on the opponents side are captured and put in that players Mancala,
 *		along with the stone that was dropped in the player's pit. If last stone is dropped
 *		into the player's Mancala, they get to go again.
 * Short Note: This function is defined before it is used, so no prototype is needed.
 */
void lastStonePlacement(int& player, int pitNum) {
	if (player == 1 && pitNum == 6) cout <<
		"Your last stone is in your Mancala, go again" << endl;
	else if (player == 2 && pitNum == 13) cout <<
		"Your last stone is in your Mancala, go again" << endl;
	else {
		int oppPit = 12 - pitNum;
		if (board[pitNum] == 1 && pitNum >= players[player].lowPit
			&& pitNum <= players[player].highPit && board[oppPit] > 0) {
				board[players[player].mancala] += board[pitNum] + board[oppPit];
				board[pitNum] = board[oppPit] = 0;
				cout << "Congratulations, you captured stones & have "
					<< board[players[player].mancala] << " now." << endl;
		}
		player == 1 ? player = 2 : player = 1;	// Switch players.
	}
}

/* void playerMove(int &player) - from rendition 3
 * Output: Modified player
 * Input: int &player (1 or 2)
 * Prupose: Do the actual moves for each player.
 */
void playerMove(int& player) {
	string readIn;					// Read a string to prrevent integer read problems.
	int pitNum = -1,				// Which pit I am choosing
		stones = 0,					// How many stones are in the chosen pit.
		emptySide = 0;				// Flag for one side or the other being empty.

	system("cls");
	printBoard();
	cout << "It is now player " << players[player].name << "'s turn.\n"
		<< "Choose between pits " << players[player].lowPit << '-'
		<< players[player].highPit << endl;
	// Loop
	do {
		cout << "Choose: ";
		/* String and integer reads
		 * I really want to read an integer here, like using the line:
		 * cin >> pitNum;
		 * But this does not work because the read does not cause an
		 * exception to be thrown when a letter is entered. Therefore,
		 * I am reading a string and then "trying" to convert it to an
		 * integer.
		 */
		cin >> readIn;
		try {
			pitNum = stoi(readIn);
		}
		// Handle the input of a character or string instead of an integer
		catch (exception e) {
			cout << "Please type a number." << endl;
			pitNum = players[player].highPit + 1;	// Flag a problem, force reread.
		}
		// Check for an empty pit to clean up problems for code following.
		if (board[pitNum] <= 0) {
			cout << "Please choose a pit that contains stones." << endl;
			pitNum = players[player].highPit + 1;	// Flag a problem, force reread.
		}
	} while (pitNum < players[player].lowPit || pitNum > players[player].highPit);

	/* NOTE: A comparison of the results of an equation can be made to 0. I do not
	 * want the user to try to pick up no stones.
	 */
	if ((stones = board[pitNum]) > 0) {				// Load number of stones and check for any
		board[pitNum] = 0;							// All the stones were picked up.
	}
	while (stones > 0) {							// If no stones, there can be no placement.
		pitNum++;
		if (pitNum > 13) pitNum = 0;
		if (player == 1 && pitNum == 13) continue;
		if (player == 2 && pitNum == 6) continue;
		board[pitNum]++;
		stones--;
	}
	// Check where the last stone is placed and switch users appropriately.
	lastStonePlacement(player, pitNum);
	emptySide = checkEmptySide();
	// Now gather all the stones from the opponent's side & add to opponent's Mancala
	if (emptySide > 0) {
		int oppSide = emptySide == 1 ? 2 : 1;
		int man = players[oppSide].mancala;
		for (int i = players[oppSide].lowPit; i <= players[oppSide].highPit; i++) {
//			cout << "Pit " << man << " is " << board[man] << endl;
//			printBoard();
			board[man] += board[i];
			board[i] = 0;						// Make sure stones removed from pits
		}
//		cout << "Pit " << man << " is " << board[man] << endl;

		player = 0;								// Flag that the game is over.
		cout << "The final board is:" << endl;
		printBoard();
	}
}

/* int checkEmptySide() - from rendition 3
 * Output: Returns the number of the player whose side is empty
 * Input: None
 */
int checkEmptySide() {
	int retVal = 0,								// Defaults to zero if neither side is empty.
		pitnum,									// Track what pit is being checked.
		i;										// Index
	for (i = 1; i <= 2; i++) {
		for (pitnum = players[i].lowPit; pitnum <= players[i].highPit; pitnum++) {
			if (board[pitnum] != 0) break;
		}
		if (pitnum >= players[i].mancala) retVal = i;
	}
	return retVal;
}

/* int findWinner() - from rendition 4
 * Output: Returns the number of the player who won the game.
 * Input: None
 * Purpose: Determine the winner and update the statistics.
 */
int findWinner() {
	int retVal = 1;
	if (board[6] > board[13]) {
		cout << players[1].name << " won!" << endl;
		players[1].wins++;
		players[2].losses++;
		retVal = 1;
	}
	else if (board[6] < board[13]) {
		cout << players[2].name << " won!" << endl;
		players[2].wins++;
		players[1].losses++;
		retVal = 2;
	}
	else {
		cout << players[1].name << " and " << players[2].name << " tied!" << endl;
		players[1].ties++;
		players[2].ties++;
		retVal = 0;
	}
	// Print what the current users statistics are
	printPlayerStatistics();
	return retVal;
}

/* int storeStatistics() - from rendition 5
 * Output: None
 * Input: None
 * Purpose: Write all the statistics to a file.
 */
void storeStatistics() {
	string transferData;
	ifstream inFile;
	ofstream outFile;
	outFile.open(fileNameBase + ".txt");
	inFile.open(fileNameBase + ".bak");
	if (outFile && inFile) {									// Only add in backup data if both files exist
		while (getline(inFile, transferData)) outFile << transferData << endl;
	}
	// Now write the data from the current users.
	for (int i = 1; i <= 2; i++) {
		outFile << players[i].name << " " << players[i].games << " " << players[i].wins <<
			" " << players[i].losses << " " << players[i].ties << endl;
	}
}
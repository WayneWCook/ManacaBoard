/* Board.cpp
 * Wayne Cook
 * 22 September 2020
 * Create the start of the Board file for the Mancala game.
 */

//Libraries to include
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

int board[] = { 4,4,4,4,4,4,0,4,4,4,4,4,4,0 };   // Set up board with stones in side pits.

/* bool solidLine(int, bool)
 *   Output: returns a boolean on success of the function
 *   Input:
 *     int length - How long do I want the line.
 *     bool newLine - whether I want a new line or not.
 */
bool solidLine(int length, bool newLine) {
	bool retVal = true;
	for (int i = 0; i < length; i++) cout << '*';
	if (newLine) cout << endl;
	return retVal;
}

/* bool dottedLine(int, bool)
 *   returns a boolean on success of the function
 *   Input:
 *     int length - How many dots do I want the line.
 *     bool newLine - whether I want a new line or not.
 */
bool dottedLine(int length, bool newLine) {
	bool retVal = true;
	for (int i = 0; i < length; i++) cout << "*    ";
	if (newLine) cout << '*' << endl;
	return retVal;
}

// The main program start
int main() {
	solidLine(41, true);
	dottedLine(8, true);
	dottedLine(1, false);
	solidLine(30, false);
	dottedLine(1, true);
	dottedLine(8, true);
	solidLine(41, true);
}
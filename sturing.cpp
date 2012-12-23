/*!
 * \file		sturing.cpp
 * \brief		sturing - Slimak Turing Machine Simulator
 * \description	Software Liberation Maniacs Krakow Turing Machine Interpreter and Simulator
 * \version		0.1212
 * \date		2012-09-11 - 2012-12-20
 * \author		Konrad Talik
 * \license		GNU General Public License
 */

#define VERSION 0.1212

#include <iostream>
#include <cstdlib>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

#include "mac/turingmachine.hpp"

using namespace std;
using namespace sturing;

const char* _prompt = ">>> ";

/**
 * Current interpreter mode enum.
 */
enum machineMode {
	STATE, 		/**< Machine state and state name declaration mode. */
	ENCOUNTER, 	/**< A character encounter in current state mode. */
	WRITE, 		/**< A character writing mode. */
	MOVE, 		/**< A head movement mode. */
	JUMP		/**< A jumping into next state mode. */
};


/*
 * Main container of states and rules.
 */
TuringMachine machine;


/*
 * Global variables for options.
 */
bool _interactive = true;
bool _echoMode = false;
bool _printLines = false;
bool _verbose = false;

/*****************//**
 * Standard messaging.
 ********************/

/**
 * Printing an error message.
 */
void printError(string file, int line, string contents) {
	if (_interactive) {
		cout << "[E] Error: " << contents << '\n';
	} else {
		cout << file << ':' << line << ": error: " << contents << '\n';
		exit(-line);
	}
};

/**
 * Printing a verbose text message.
 */
void verbosePrint(string contents) {
	cout << "[I] " << contents << '\n';
};


/**
 * Printing a board of states.
 */
void printBoard() {

	cout << "|*|" << "   " << machine.getState(1) << "   ";
	
	for (int s = 2; s <= machine.numberOfStates(); ++s) {
		cout <</* "|*" <<*/ "|   " << machine.getState(s) << "   ";
	};

	cout << '|' << endl;
	
	cout << "|-|";
	
	for (int s = 1; s <= machine.numberOfStates(); ++s) {
		if (s > 1)
			cout << "+";//"|-|";
		cout << "-------";
	};

	cout << '|' << endl;

	for (int i = 1; i <= machine.numberOfCharacters(); ++i) {

			cout << '|';

			for (int j = 1; j <= machine.numberOfStates(); ++j) {

					if (j == 1) {
						cout <<  machine.getCharacter(i) << "| ";
					} else {
						cout << " ";
					}

					if ( machine[j][i].write != 0)
						cout << machine.getCharacter( machine[j][i].write ) << ' ';
					else
						cout << "- ";

					switch ( machine[j][i].move  ) {

						case LEFT:
							cout << '<';
						break;

						case RIGHT:
							cout << '>';
						break;

						case STAY:
							cout << '=';
						break;

						case NONE:
							cout << '-';

					}
					
					cout << ' ';

					if (machine[j][i].jump > 0)
						cout << machine.getState( machine[j][i].jump );
					else if (machine[j][i].jump == 0)
						cout << '-';
					else if (machine[j][i].jump == -1)
						cout << '^';

					cout << " |";

			}

			cout << endl;

	}

};


int main(int argc, char** argv) {

	/*
	 * Program arguments handling
	 */

	bool _onlyBoard = false;

	string _fileName;
	
	if (argc > 1) {
		string argument;
		
		for (int i=1; i<argc; ++i) {
			argument = argv[i];
			
			// Version printing.
			if ( argument.compare("-v") == 0 || argument.compare("--version") == 0 ) {
				cout << VERSION << '\n';
				return 0;
			
			// Echo mode - source code printing.
			} else if ( argument.compare("-e") == 0 || argument.compare("--echo") == 0 ) {
				_echoMode = true;

			// Echo with lines numbers.
			} else if ( argument.compare("-l") == 0 || argument.compare("--echo-lines") == 0 ) {
				_echoMode = true;
				_printLines = true;

			// Be verbose.
			} else if (argument.compare("-b") == 0 || argument.compare("--verbose") == 0) {
				_verbose = true;

			// Print the table and end the program.
			} else if (argument.compare("-t") == 0 || argument.compare("--state-board") == 0) {
				_onlyBoard = true;

			// Unknown option - source file.
			} else {
				
				// Try to open the file
				int source = open(argument.c_str(), ios::in);
				if (source < 0) {
					cout << "[E] No such file or directory: " << argument << ".\n";
					return 1;

				// Redirect standard input to file
				} else if ( dup2(source,0) < 0) {
					cout << "[E] Error reading file " << argument << ".\n";
					return 2;
				}

				// Done opening
				_interactive = false;
				_fileName = argument;
			}
		}
	}

	/*
	 * Interpreter startup - welcome message if interactive.
	 */

	if (_interactive) {
		cout << "STuring " << VERSION << " - Turing Machine interpreter.\n";
		cout << _prompt;
	}

	/*
	 * INTERPRETER
	 */

	string line;
	int _lineNumber = 1;
	
	string word;
	int found;
	int length;

	// Auxiliary variables
	string currentState = "";
	// 0 - no state. 1 - first state set in while.
	int currentStateIndex = 0;
	string currentCharacter = "";
	int currentCharacterIndex = 0;
	string currentWrite = "";
	int currentWriteIndex = 0;
	string currentDirectionWord = "";
	direction currentDirection = NONE;
	string currentJump = "";
	int currentJumpIndex = 0;
	
	// Starting with first state interpretation:
	machineMode currentMode = STATE;

	int i;

	// Interpreting every line
	while (getline(cin,line) && line.compare("exit") && line.compare("go") && line.compare("GO")) {

		// Before interpreting
		if (_echoMode) {
			if (_printLines) {
				cout << _lineNumber << ": ";
			}
			cout << line << "\n";
		}

		// Begin interpreting
		
		// Every word in line
		line += ' ';
		found = -1;
		length = 0;
		for (i = 0; i < line.size(); ++i) {
			if (found > -1) {
				if (isspace(line[i])) {
					
					word = line.substr(found,length);
					
					// Interpreting word
					
					switch (currentMode) {

						// State definition.
						case STATE:

							if (word[length-1] == ':' ) {
								
								currentState = word.substr(0,length-1);

								currentStateIndex = machine.declareState(currentState);
								currentMode = ENCOUNTER;

								if (_verbose) {	
									verbosePrint("Starting a new definition of the state '" + currentState + "'.");
								}
							
							} else {

								printError(_fileName, _lineNumber, 
									"Expected state definition beginning. Missing ':' symbol?");
							
							}
							
						break;

						// Encounter of a character.
						case ENCOUNTER:

							currentCharacter = word;
								
								// Next state definition started.
								if ( word[length-1] == ':') {
								
									//TODO: No characters to encounter are given in state definition
									// and next state definition started - syntax error?

									currentState = word.substr(0,length-1);
									currentStateIndex = machine.declareState(currentState);
									currentMode = ENCOUNTER;

								} else {

									// Check the index of character, or - if not exist - add to alphabet.
									currentCharacterIndex = machine.declareCharacter(currentCharacter);
									currentMode = WRITE;

								}	
							

						break;


						case WRITE:

							currentWrite = word;
							currentWriteIndex = machine.declareCharacter(word);
							currentMode = MOVE;
							
							if (_verbose) {
								verbosePrint("Setting to write a character '" + currentWrite + "' within this rule.");
							}

						break;

						case MOVE:

							currentDirectionWord = word;

							if ( word[0] == '>'
								|| word[0] == 'R'
								|| word[0] == 'P'
								|| word.compare("stright") == 0) {

								currentDirection = RIGHT;
								currentMode = JUMP;
								
								if (_verbose) {
									verbosePrint("Setting to move machine head right within this rule.");
								}

							} else if ( word[0] == '<'
								|| word[0] == 'L'
								|| word.compare("stleft") == 0) {

								currentDirection = LEFT;
								currentMode = JUMP;
								
								if (_verbose) {
									verbosePrint("Setting to move machine head left within this rule.");
								}

							} else if ( word[0] == '='
								|| word[0] == 'S'
								|| word.compare("ststay") == 0) {

								currentDirection = STAY;
								currentMode = JUMP;
								
								if (_verbose) {
									verbosePrint("Setting to not move machine head within this rule.");
								}

							}

						break;

						case JUMP:

							currentJump = word;

							// Turn off machine.
							if ( word[0] == '^' || word[0] == ';' || word.compare("stdone") == 0) {

								currentJumpIndex = -1;
								
								if (_verbose) {	
									verbosePrint("Setting to end the machine process.");
								}

							// Jump to the state.
							} else {

								currentJumpIndex = machine.declareState(currentJump);

							}

							TuringStateRule newRule;
							newRule.write = currentWriteIndex;
							newRule.move = currentDirection;
							newRule.jump = currentJumpIndex;

							machine[ currentStateIndex ][ currentCharacterIndex ] = newRule;
									
							currentMode = ENCOUNTER;

						break;
					}
					// Word interpreted
					
					found = -1;
					length = 0;
				
				} else {
					++length;
				}
			
			} else if (!isspace(line[i])) {
				found = i;
				++length;
			}
		}

		// End interpreting

		// After interpreting
		if (_interactive) {
			cout << _prompt;
		}

		++_lineNumber;
	}

	// EOF or 'exit' command.
	
	if (_onlyBoard) {

		printBoard();
		return 0;

	}
	
	return 0;
}

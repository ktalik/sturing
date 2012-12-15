/* 
 * sturing - Slimak Turing Machine Simulator
 * 2012-09-11 Konrad Talik
 */

#define VERSION 0.1212

#include <cstring>
#include <string>
#include <cstdlib>
#include <ctype.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>

using namespace std;

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

/**
 * Direction of head movement.
 */
enum direction { LEFT, RIGHT, STAY };

/**
 * Turing machine state node struct.
 * This nodes should be indexed by encountered character number.
 * Node includes all information for every step of Turing Machine:
 * character to be written, which direction to move, in which state to jump.
 * write == -1 - no character
 * jump == -1 - no state change
 */
struct STuringStateNode {
	int write;
	machineMode move;
	int jump;
};

void printError(bool interactive, string file, int line, string contents) {
	if (interactive) {
		cout << "Error: " << contents << '\n';
	} else {
		cout << file << ':' << line << ": error: " << contents << '\n';
		exit(-line);
	}
};


int main(int argc, char** argv) {

	/*
	 * Program arguments handling
	 */

	bool _interactive = true;
	bool _echoMode = false;
	bool _printLines = false;

	string _fileName;
	
	if (argc > 1) {
		string argument;
		
		for (int i=1; i<argc; ++i) {
			argument = argv[i];
			
			// Version printing
			if ( argument.compare("-v") == 0 || argument.compare("--version") == 0) {
				cout << VERSION << '\n';
				return 0;
			
			// Echo mode
			} else if ( argument.compare("-e") == 0 || argument.compare("--echo") == 0) {
				_echoMode = true;

			// Echo with lines numbers
			} else if ( argument.compare("-l") == 0 || argument.compare("--echo-lines") == 0) {
				_echoMode = true;
				_printLines = true;

			// Unknown option - source file
			} else {
				
				// Try to open the file
				int source = open(argument.c_str(), ios::in);
				if (source < 0) {
					cout << "No such file or directory: " << argument << ".\n";
					return 1;

				// Redirect standard input to file
				} else if ( dup2(source,0) < 0) {
					cout << "Error reading file " << argument << ".\n";
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

	// Main container of states, and main variables
	vector<STuringState> TuringMachine;
	int *alphabet;

	string line;
	int _lineNumber = 1;
	
	string word;
	int found;
	int length;
	
	// Starting with first state interpretation:
	machineMode currentMode = STATE;
	// -1 - no state. 0 - first state set in while.
	int stateNumber = -1;
	// At the beginning the alphabet is unknown:
	bool unknownAlphabet = true;
	int alphabetCount = 0;

	int i;

	// Interpreting every line
	while(getline(cin,line) && line.compare("exit")){

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

						case STATE:
						
							if (word[length-1] == ':') {
								
								string newState = word.substr(0,length-1);
								currentMode = ENCOUNTER;
							
							} else {

								printError(_interactive, _fileName, _lineNumber, 
									"Expected state definition beginning. Missing ':' symbol?");
							
							}

						break;

						case ENCOUNTER:

							

						break;

						case WRITE:

						break;

						case MOVE:

						break;

						case JUMP:

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
	
	return 0;
}

/*!
 * \file		sturing.cpp
 * \brief		sturing - Simple Turing Machine / Slimak Turing Machine
 * \description	Simple Turing Machine Interpreter and Simulator
 * \version		0.1212
 * \date		2012-09-11 - 2012-12-28
 * \author		Konrad Talik
 * \license		GNU General Public License
 */

#define VERSION 0.1212

#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

#include "mac/turingmachine.hpp"
#include "uti/optionscontainter.hpp"
#include "int/printer.hpp"

using namespace std;
using namespace sturing;

const char* _prompt = ">>> ";

/*
 * Main container of states and rules.
 */
TuringMachine machine;

//! Variables for options.
OptionsContainter options;

//! Printing messages.
Printer printer;

int main(int argc, char** argv) {

	options.interactive = true;
	options.echoMode = false;
	options.printLines = false;
	options.verbose = false;
	options.onlyBoard = false;
	options.initialTape = false;
	options.noSpaces = false;

	/*
	 * Program arguments handling
	 */

	string _fileName;
	bool sourceLoaded = false;
	
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
				options.echoMode = true;

			// Echo with lines numbers.
			} else if ( argument.compare("-l") == 0 || argument.compare("--echo-lines") == 0 ) {
				options.echoMode = true;
				options.printLines = true;

			// Be verbose.
			} else if (argument.compare("-b") == 0 || argument.compare("--verbose") == 0) {
				options.verbose = true;

			// Print the table and end the program.
			} else if (argument.compare("-t") == 0 || argument.compare("--state-board") == 0) {
				options.onlyBoard = true;

			// Print the intial tape.
			} else if (argument.compare("-i") == 0 || argument.compare("--initial-tape") == 0) {
				options.initialTape = true;

			// Print the tape without spaces.
			} else if (argument.compare("-s") == 0 || argument.compare("--no-spaces") == 0) {
				options.noSpaces = true;

			// Unknown option - source file.
			} else if (not sourceLoaded) {
				
				// Try to open the file
				int source = open(argument.c_str(), ios::in);
				if (source < 0) {
					cout << "[E] No such source file or directory: '" << argument << "'.\n";
					return 1;

				// Redirect standard input to file
				} else if ( dup2(source,0) < 0) {
					cout << "[E] Error reading source file '" << argument << "'.\n";
					return 2;
				}

				// Done opening
				options.interactive = false;
				_fileName = argument;
				sourceLoaded = true;
			
			// If source loaded - tape file.
			} else {
				
				machine.loadTape( argument );

			}
		}
	}

	machine.setOptions(options.verbose, options.noSpaces);

	if (options.initialTape)
		machine.printTape();

	/*
	 * Interpreter startup - welcome message if interactive.
	 */

	if (options.interactive) {
		cout << "STuring " << VERSION << " - Simple Turing Machine interpreter.\n";
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

	// Interpreting every line.
	while (getline(cin,line) && line.compare("exit") && line.compare("go") && line.compare("GO")) {

		// Before interpreting.
		if (options.echoMode) {
			if (options.printLines) {
				cout << _lineNumber << ": ";
			}
			cout << line << "\n";
		}

		// Begin interpreting.
		
		// Every word in line.
		line += ' ';
		found = -1;
		length = 0;
		for (i = 0; i < line.size(); ++i) {
			if (found > -1) {
				if (isspace(line[i])) {
					
					word = line.substr(found,length);

					// If its a comment.
					if ( word[0] == '/' && word[1] == '/') {
							continue;
					}
					
					// Else, interpreting word.
					
					switch (currentMode) {

						// State definition.
						case STATE:

							if (word[length-1] == ':' ) {
								
								currentState = word.substr(0,length-1);
								currentStateIndex = machine.declareState(currentState);
								currentMode = MEETING;

								if (options.verbose) {	
									printer.verbosePrint("[NEW STATE DEF] Starting a new definition of the state '" + currentState + "'.");
								}
							
							} else {

								printer.printError(_fileName, _lineNumber, 
									"Expected state definition beginning. Missing ':' symbol?");
							
							}
							
						break;

						// Encounter of a character.
						case MEETING:

							currentCharacter = word;
								
								// Next state definition started.
								if ( word[length-1] == ':') {
								
									//TODO: No characters to encounter are given in state definition
									// and next state definition started - syntax error?

									currentState = word.substr(0,length-1);
									currentStateIndex = machine.declareState(currentState);
									currentMode = MEETING;
								
									if (options.verbose) {	
										printer.verbosePrint("[NEW STATE DEF] Starting a new definition of the state '" + currentState + "'.");
									}

								} else {

									// Check the index of character, or - if not exist - add to alphabet.
									currentCharacterIndex = machine.declareCharacter(currentCharacter);
									currentMode = WRITE;
							
									if (options.verbose) {
										printer.verbosePrint("[MEETING] Starting a new rule with meeting the character '" + currentCharacter + "'.");
									}

								}	
							

						break;


						case WRITE:

							currentWrite = word;

							if ( word.compare("\\=") == 0) {
								currentWriteIndex = -1;
							
							} else if ( word.compare("\\tape" ) == 0) {
								currentWriteIndex = 0;

							} else {
								currentWriteIndex = machine.declareCharacter(word);
							
							}

							currentMode = MOVE;
							
							if (options.verbose) {
								printer.verbosePrint("[WRITE] Setting to write a character '" + currentWrite + "' within this rule.");
							}

						break;

						case MOVE:

							currentDirectionWord = word;

							if ( word[0] == '>'
								|| word[0] == 'P' || word[0] == 'p'
								|| word[0] == 'R' || word[0] == 'r'
								|| word.compare("\\right") == 0) {

								currentDirection = RIGHT;
								currentMode = JUMP;
								
								if (options.verbose) {
									printer.verbosePrint("[MOVE] Setting to move machine head right within this rule.");
								}

							} else if ( word[0] == '<'
								|| word[0] == 'L' || word[0] == 'l'
								|| word.compare("\\left") == 0) {

								currentDirection = LEFT;
								currentMode = JUMP;
								
								if (options.verbose) {
									printer.verbosePrint("[MOVE] Setting to move machine head left within this rule.");
								}

							} else if ( word[0] == '='
								|| word[0] == 'S' || word[0] == 's'
								|| word.compare("\\stay") == 0) {

								currentDirection = STAY;
								currentMode = JUMP;
								
								if (options.verbose) {
									printer.verbosePrint("[MOVE] Setting to not move machine head within this rule.");
								}

							}

						break;

						case JUMP:

							currentJump = word;

							// Turn off machine.
							if ( word[0] == '^' || word[0] == ';' || word.compare("\\done") == 0) {

								currentJumpIndex = -1;
								
								if (options.verbose) {	
									printer.verbosePrint("[JUMP] Setting to end the machine process within this rule.");
								}

							// Jump to the state.
							} else {

								currentJumpIndex = machine.declareState(currentJump);

								if (options.verbose) {	
									printer.verbosePrint("[JUMP] Setting to jump to the state '" + machine.getState(currentJumpIndex) + "' within this rule.");
								}

							}

							TuringStateRule newRule;
							newRule.write = currentWriteIndex;
							newRule.move = currentDirection;
							newRule.jump = currentJumpIndex;

							machine[ currentStateIndex ][ currentCharacterIndex ] = newRule;
									
							currentMode = MEETING;

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
		if (options.interactive) {
			cout << _prompt;
		}

		++_lineNumber;
	}

	// EOF or 'exit' command.
	
	if (options.onlyBoard) {

		printer.printBoard();
		return 0;

	}
	
	return 0;
}

/**
 * @file	machine/turingmachine.cpp
 */

#include "machine/turingmachine.hpp"

using namespace sturing;

TuringMachine::TuringMachine() {
    options = new OptionsContainer();
	declareCharacter("_");
	tape.push_front(1);
	head = tape.begin();
	tape.push_front(1);
}

TuringMachine::~TuringMachine() {

}

void TuringMachine::setOptions(OptionsContainer *givenOptions) {
    if (options) {
        delete options;
    }
	options = givenOptions;
}

void TuringMachine::setPrinter(Printer *givenPrinter) {
	printer = givenPrinter;
}

int TuringMachine::declareCharacter(std::string givenCharacter) {

	int characterIndex;

	// Character does not exist.
	if ( characterToIndex[givenCharacter] == 0 ) {

		characterIndex = characterToIndex.size();
		characterToIndex[givenCharacter] = characterIndex;
		indexToCharacter[characterIndex] = givenCharacter;

		if (options->verbose) {
			printer->verbosePrint("[DECLARATION] Declaration of a new character '" + givenCharacter + "'.");
		}

	} else {

		characterIndex = characterToIndex[givenCharacter];

	}

	return characterIndex;
}

int TuringMachine::declareState(std::string givenState) {

	int stateIndex;

	// State does not exist.
	if ( stateToIndex[givenState] == 0 ) {

		stateIndex = stateToIndex.size();
		stateToIndex[givenState] = stateIndex;
		indexToState[stateIndex] = givenState;

		if (options->verbose) {
			printer->verbosePrint("[DECLARATION] Declaration of a new state '" + givenState + "'.");
		}


	} else {

		stateIndex = stateToIndex[givenState];

	}

	return stateIndex;
}

std::string TuringMachine::getCharacter(int index) {
	if (index == -1)
		return "\\=";
	else
		return indexToCharacter[index];
}

std::string TuringMachine::getState(int index) {
	return indexToState[index];
}

void TuringMachine::run() {

	int currentStateIndex = 1;
	TuringStateRule currentRule;

	while( true ) {

		if (!options->hideSteps) {
			printTape();
			if (options->printHead) {
				std::cout << "HEAD: " <<  getCharacter(*head) << "\n\n";
			}
		}

		currentRule = machine[ currentStateIndex ][ *head ];
		putOnTape(currentRule.write);

		if (currentRule.move == LEFT) {
			moveLeft();

		} else if (currentRule.move == RIGHT) {
			moveRight();
		}

		if (currentRule.jump == -1) {
			return;

		} else {
			currentStateIndex = currentRule.jump;

		}

	}

}

std::map<int, TuringStateRule>& TuringMachine::operator[](size_t index) {
	return machine[index];
}

const std::map<int, TuringStateRule> TuringMachine::operator[](size_t index) const {
	//TODO
}

int TuringMachine::numberOfCharacters() {
	return indexToCharacter.size();
}

int TuringMachine::numberOfStates() {
	return indexToState.size();
}

void TuringMachine::loadTape(std::string givenTapeFileName) {

	std::ifstream tapeFile( givenTapeFileName.c_str() );
	std::string tapeString( (std::istreambuf_iterator<char>(tapeFile)), (std::istreambuf_iterator<char>()) );
	std::string word;

	// Every word in line
	tapeString += ' ';
	int found = -1;
	int length = 0;

	for (int i = 0; i < tapeString.size(); ++i) {

		if (found > -1) {

			if (isspace(tapeString[i])) {

				word = tapeString.substr(found,length);
				putOnTape( declareCharacter( word ) );
				moveRight();
				found = -1;
				length = 0;

			} else {
				++length;
			}

		} else if (!isspace(tapeString[i])) {
			found = i;
			++length;
		}
	}

	head = tape.begin();
	++head;

}

void TuringMachine::putOnTape(int index) {
	*head = index;
}

void TuringMachine::printTape() {
	for (std::list<int>::iterator it = tape.begin(); it != tape.end(); ++it) {
		std::cout << getCharacter(*it);
		if ( !options->noSpaces )
			std::cout << ' ';
	}
	std::cout << std::endl;
}

void TuringMachine::moveRight() {
	// Check, what if we increment the head.
	std::list<int>::iterator nextHead = head;
	++nextHead;
	if(nextHead == tape.end()) {
		tape.push_back(1);
	}
	++head;
}

void TuringMachine::moveLeft() {
	// Check, what if we decrement the head.
	std::list<int>::iterator nextHead = head;
	--nextHead;
	if(nextHead == tape.begin()) {
		tape.push_front(1);
	}
	--head;
}

void TuringMachine::printBoard() {

	cout << "|*|" << "   " << getState(1) << "   ";

	for (int s = 2; s <= numberOfStates(); ++s) {
		cout << "|   " << getState(s) << "   ";
	};

	cout << '|' << endl;

	cout << "|-|";

	for (int s = 1; s <= numberOfStates(); ++s) {
		if (s > 1)
			cout << "+";
		cout << "-------";
	};

	cout << '|' << endl;

	for (int i = 1; i <= numberOfCharacters(); ++i) {

			cout << '|';

			for (int j = 1; j <= numberOfStates(); ++j) {

					if (j == 1) {
						cout <<  getCharacter(i) << "| ";
					} else {
						cout << " ";
					}

					if ( machine[j][i].write != 0)
						cout << getCharacter( machine[j][i].write ) << ' ';
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

					if ( machine[j][i].jump > 0 )
						cout << getState( machine[j][i].jump );
					else if ( machine[j][i].jump == 0 )
						cout << '-';
					else if ( machine[j][i].jump == -1)
						cout << '^';

					cout << " |";

			}

			cout << endl;

	}

}

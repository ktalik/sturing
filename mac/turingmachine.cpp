/*!
 * \file		mac/turingmachine.cpp
 */

#include "mac/turingmachine.hpp"

using namespace sturing;

TuringMachine::TuringMachine() {
	head = tape.begin();
	declareCharacter("_");
}

TuringMachine::~TuringMachine() {

}

int TuringMachine::declareCharacter(std::string givenCharacter) {
	
	int characterIndex;

	// Character does not exist.
	if ( characterToIndex[givenCharacter] == 0 ) {

		characterIndex = characterToIndex.size();
		characterToIndex[givenCharacter] = characterIndex;
		indexToCharacter[characterIndex] = givenCharacter;

		/*if (_verbose) {
			verbosePrint("Declaration of a new character '" + givenCharacter + "'.");
		}
		}*/


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

		/*if (_verbose) {	
				verbosePrint("Declaration of a new state '" + givenState + "'.");
		}*/


	} else {

		stateIndex = stateToIndex[givenState];
									
	}

	return stateIndex;
}

std::string TuringMachine::getCharacter(int index) {
	return indexToCharacter[index];
}

std::string TuringMachine::getState(int index) {
	return indexToState[index];
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

	// Init the tape.
	tape.push_front(1);
	head = tape.begin();
	tape.push_front(1);

	// Every word in line
	tapeString += ' ';
	int found = -1;
	int length = 0;
	//TODO: Place this function in an interpreter class.
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

}

void TuringMachine::putOnTape(int index) {
	*head = index;
}

void TuringMachine::printTape() {
	for (std::list<int>::iterator it = tape.begin(); it != tape.end(); ++it) {
		std::cout << getCharacter(*it) << ' ';
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

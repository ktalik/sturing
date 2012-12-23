/*!
 * \file		mac/turingmachine.cpp
 */

#include "mac/turingmachine.hpp"

using namespace sturing;

TuringMachine::TuringMachine() {
	
}

TuringMachine::~TuringMachine() {

}

int TuringMachine::declareCharacter(std::string givenCharacter) {
	
	int characterIndex;

	// State does not exist.
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

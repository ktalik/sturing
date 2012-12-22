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
				verbosePrint("Starting a definition of a new state '" + currentState + "'.");
		}*/


	} else {

		characterIndex = characterToIndex[givenCharacter];
									
		/*if (_verbose) {	
				verbosePrint("Starting a new definition of the existing state '" + currentState + "'.");
		}*/

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
				verbosePrint("Starting a definition of a new state '" + currentState + "'.");
		}*/


	} else {

		stateIndex = stateToIndex[givenState];
									
		/*if (_verbose) {	
				verbosePrint("Starting a new definition of the existing state '" + currentState + "'.");
		}*/

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

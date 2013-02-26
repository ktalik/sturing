/**
 * @file 	mac/turingmachine.hpp
 * @brief	Turing Machine implementation and its help structures.
 * @date	2012-12-20 - 2013-02-26
 * @author	Konrad Talik
 * @copyright	GNU General Public License v3.
 */

#ifndef __STURINGMACHINE__
#define __STURINGMACHINE__

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>

namespace sturing {


/**
 * Current interpreter mode enum.
 */
enum machineMode {
	STATE, 		/**< Machine state and state name declaration mode. */
	MEETING, 	/**< A character encounter in current state mode. */
	WRITE, 		/**< A character writing mode. */
	MOVE, 		/**< A head movement mode. */
	JUMP		/**< A jumping into next state mode. */
};


/**
 * Direction of head movement.
 */
enum direction { NONE, LEFT, RIGHT, STAY };


/**
 * @brief	Turing Machine state rule struct.
 * 
 * Rule includes all information for every step of Turing Machine:
 * character to be written, which direction to move, in which state to jump.
 */
struct TuringStateRule {
	int write;
	direction move;
	int jump;
};


	/**
	 * @brief	A Turing Machine implementation.
	 * @date	2012-12-20
	 * @author	Konrad Talik
	 * @copyright	GNU Public License.
	 */
	class TuringMachine {

	private:

		/// Main container of rules in states.
		std::map <int, std::map <int, TuringStateRule> > machine;

		/// Machine tape.
		std::list<int> tape;
		std::list<int>::iterator head;

		/// Double-side maping of characters.
		std::map<int, std::string> indexToCharacter;
		/// Double-side maping of characters.
		std::map<std::string, int> characterToIndex;

		/// Double-side maping of states.
		std::map<int, std::string> indexToState;
		/// Double-side maping of states.
		std::map<std::string, int> stateToIndex;

		/// Options.
		bool _noSpaces, _verbose;

	public:

		TuringMachine();
		~TuringMachine();

		/**
		 * @brief	Eventually inserts a character into an alphabet.
		 * @return	The character index.
		 */
		int declareCharacter(std::string character);
	
		/**
		 * @brief	Eventually inserts a new state into the Machine.
		 * @param	givenState	Current state name.
		 * @return	The state index.
		 *
		 * If state does not exist (is not declared) creates a state and assigns to it an unique index.
		 * Otherwise the index is taken from the dictionary.
		 */
		int declareState(std::string givenState);

		/**
		 * @return	The character symbol.
		 */
		std::string getCharacter(int index);
	
		/**
		 * @return	The state symbol.
		 */
		std::string getState(int index);

		/**
		 * Index operator.
		 */
		std::map<int, TuringStateRule>& operator[](size_t index);
		const std::map<int, TuringStateRule> operator[](size_t index) const;

		int numberOfCharacters();
		int numberOfStates();

		void loadTape(std::string givenTapeFileName);
		void putOnTape(int index);
		void printTape();
		void moveRight();
		void moveLeft();

		void setOptions(bool verbose, bool noSpaces);

	};

}
#endif

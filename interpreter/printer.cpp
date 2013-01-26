/*!
 * \file		int/printer.cpp
 * \date		2012-01-07 - 2012-01-17
 * \author		Konrad Talik
 * \copyright	GNU General Public License.
 */

#include "interpreter/printer.hpp"
#include <cstdlib>
#include <iostream>

using namespace sturing;

Printer::Printer() : options(new OptionsContainter()), machine(new TuringMachine()) {

}

Printer::~Printer() {

}

void Printer::printError(string file, int line, string contents) {
	if (options->interactive) {
		cout << "[E] Error: " << contents << '\n';
	} else {
		cout << file << ':' << line << ": error: " << contents << '\n';
		exit(-line);
	}
};


void Printer::verbosePrint(string contents) {
	cout << "[I] " << contents << '\n';
};


void Printer::printBoard() {

	cout << "|*|" << "   " << machine->getState(1) << "   ";
	
	for (int s = 2; s <= machine->numberOfStates(); ++s) {
		cout <</* "|*" <<*/ "|   " << machine->getState(s) << "   ";
	};

	cout << '|' << endl;
	
	cout << "|-|";
	
	for (int s = 1; s <= machine->numberOfStates(); ++s) {
		if (s > 1)
			cout << "+";//"|-|";
		cout << "-------";
	};

	cout << '|' << endl;

	for (int i = 1; i <= machine->numberOfCharacters(); ++i) {

			cout << '|';

			for (int j = 1; j <= machine->numberOfStates(); ++j) {

					if (j == 1) {
						cout <<  machine->getCharacter(i) << "| ";
					} else {
						cout << " ";
					}

					if ( (*machine)[j][i].write != 0)
						cout << machine->getCharacter( (*machine)[j][i].write ) << ' ';
					else
						cout << "- ";

					switch ( (*machine)[j][i].move  ) {

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

					if ( (*machine)[j][i].jump > 0 )
						cout << machine->getState( (*machine)[j][i].jump );
					else if ( (*machine)[j][i].jump == 0 )
						cout << '-';
					else if ( (*machine)[j][i].jump == -1)
						cout << '^';

					cout << " |";

			}

			cout << endl;

	}

};


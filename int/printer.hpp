/*!
 * \file 		int/printer.hpp
 * \brief		Standard messaging class.
 * \date		2012-01-07 - 2012-01-17
 * \author		Konrad Talik
 * \copyright	GNU General Public License.
 */

#ifndef __STURINGPRINTER__
#define __STURINGPRINTER__

#include "mac/turingmachine.hpp"
#include "uti/optionscontainter.hpp"
#include <string>

using namespace std;

namespace sturing {

	class Printer {

	private:

		OptionsContainter	*options;
		TuringMachine		*machine;

	public:

		Printer();
		~Printer();
		
		//! Printing an error message.
		void printError(string file, int line, string contents);
		
		
		//! Printing a verbose text message.
		void verbosePrint(string contents);


		//! Printing a board of states.
		void printBoard();

	};

}

#endif

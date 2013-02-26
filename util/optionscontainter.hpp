/**
 * @brief	Program options containter.
 * @date	2013-01-07
 * @author	Konrad Talik
 * @copyright	GNU Public License v3.
 */

#ifndef __STURINGOPTIONS__
#define __STURINGOPTIONS__

/// Variables for options.
struct OptionsContainter {

	bool interactive;
	bool echoMode;
	bool printLines;
	bool verbose;
	bool onlyBoard;
	bool printInitialTape;
	bool noSpaces;

};

namespace sturing {

void printHelp();

}

#endif

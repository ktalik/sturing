/**
 * @brief	Program options containter.
 * @date	2013-01-07
 * @author	Konrad Talik
 * @copyright	GNU Public License v3.
 */

#ifndef __STURINGOPTIONS__
#define __STURINGOPTIONS__

namespace sturing {

/// Variables for options.
struct OptionsContainer {

	bool interactive;
	bool echoMode;
	bool printLines;
	bool verbose;
	bool onlyBoard;
	bool printInitialTape;
	bool noSpaces;
	bool stepByStep;

};

/// Help printing method.
void printHelp();

}

#endif

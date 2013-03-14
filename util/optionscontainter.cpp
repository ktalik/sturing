/**
 * @file	util/optionscontainter.cpp
 */

#include <util/optionscontainter.hpp>
#include <iostream>

using std::cout;

void sturing::printHelp() {

	cout << "usage: sturing [OPTIONS] [SOURCE FILE] [TAPE FILE]\n"
		<< "\n"
		<< "If no SOURCE FILE is given, interactive mode is started.\n"
		<< "If there is no TAPE FILE, the tape will be initialized as empty.\n"
		<< "\n"
		<< "  -n, --version                 Print version and end the program process.\n"
		<< "  -h, --help                    Print this help text\n"
		<< "                                and end the program process.\n"
		<< "  -e, --echo                    Print wchole source code input.\n"
		<< "  -l, --echo-lines              Same as --echo, but with line numbers.\n"
		<< "  -v, --verbose                 Be verbose while interpreting.\n"
		<< "  -b, --board                   Print a board of states of the source\n"
		<< "                                and end the program process.\n"
		<< "  -i, --print-initial-tape      Print initial loaded (or not) tape.\n"
		<< "  -s, --no-spaces               Do not print spaces between characters\n"
		<< "                                when printing a tape contents.\n"
		<< "  -d, --head                    Print current head character.\n"
		<< "  -x, --hide-steps              Disable step-by-step printing.\n"
		<< "\n"
		<< "This project is under heavy development and this is a very early alpha version.\n"
		<< "Sturing repository page: https://github.com/ktalik/sturing\n"
		<< "Please report any bugs to konradtalik at gmail.com.\n";

}


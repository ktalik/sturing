/**
 * @file    int/printer.cpp
 * @date    2012-01-07 - 2012-01-17
 * @author  Konrad Talik
 * @copyright   GNU General Public License v3.
 */

#include "interpreter/printer.hpp"
#include <cstdlib>
#include <iostream>

using namespace sturing;

Printer::Printer() : options(new OptionsContainer()) {

}

Printer::Printer(OptionsContainer *givenOptions) : options(givenOptions) {
}

Printer::~Printer() {

}

void Printer::setOptions(OptionsContainer *givenOptions) {
    options = givenOptions;
}

void Printer::printError(string file, int line, string contents) {
    if (options->interactive) {
        cout << "[E] Error: " << contents << '\n';
    } else {
        cout << file << ':' << line << ": error: " << contents << '\n';
        exit(-line);
    }
}


void Printer::verbosePrint(string contents) {
    cout << "[I] " << contents << '\n';
}




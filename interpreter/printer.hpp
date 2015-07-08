/**
 * @file    int/printer.hpp
 * @brief   Standard messaging class.
 * @date    2012-01-07 - 2012-01-17
 * @author  Konrad Talik
 * @copyright   GNU General Public License.v3
 */

#ifndef __STURINGPRINTER__
#define __STURINGPRINTER__

#include "util/optionscontainter.hpp"
#include <string>

using namespace std;

namespace sturing {

    /**
     * @brief   Standard messaging class.
     * Objects of this class are able to print messages,
     * following properties of OptionsContainer.
     */
    class Printer {

    private:

        OptionsContainer    *options;

    public:

        /// Constructs a new Printer object.
        Printer();
        /// Constructs a new Printer object with given pointer to OptionsContainter.
        Printer(OptionsContainer *givenOptions);
        ~Printer();

        /// Method used to set a pointer to OptionsContainter object.
        void setOptions(OptionsContainer *givenOptions);

        /// Printing an error message.
        void printError(string file, int line, string contents);
        /// Printing a verbose text message.
        void verbosePrint(string contents);

    };

}

#endif

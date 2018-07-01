/**************************************************
**Program Name: joeUtilities.cpp
**Author: Joe Kirkham
**Date: 2017/03/04
**Description: This is the implementation file for the joeUtilities functions.
**             It contains miscellaneous shortcut functions that are common
**             items in my coding style.
**************************************************/

#include <iostream>
#include <string>
#include <limits>
#include "joeUtilities.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::numeric_limits;
using std::streamsize;

/**************************************************
**joePrint() skips a line, then starts the next line with
**two stars and prints out the passed string argument.
**************************************************/

void joePrint(string s) {
    //cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << endl
         << "**" << s << endl;
}

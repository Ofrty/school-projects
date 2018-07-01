/*********************************************
**Program Name: menu.hpp
**Author: Joe Kirkham
**Date: 2017/01/21
**Description: This is the implementation file for the function
**             validateInputInclusive(). The purpose of this function
**             is to validate the input of a value (currently: int or long int). It validates
**             two things: first, is it even a number, and second,
**             does it fall within the specified range? The three
**             arguments are:
**             1. the input itself
**             2. the minimum acceptable value
**             3. the maximum acceptable value
**
**             CHANGELOG:
**             2017/01/29 (the cruise update) - the function now prompts the user to re-enter if invalid
**             2017/02/05 - added long int and overloads
**
**             LATER: specify name for integer input.
*********************************************/

#include <iostream>
#include "validateInputInclusive.hpp"

using std::cin;
using std::cout;
using std::endl;

//int overload
int validateInputInclusive(int input, int minimum, int maximum) {
    bool validated = true;

    do {
        validated = true;

        //is the input even an int? used with aid from http://www.learncpp.com/cpp-tutorial/185-stream-states-and-input-validation/
        if (cin.fail())
            {
                validated = false;
                cin.ignore();
            }

        //is the int within the specified range?
        if (input < minimum || input > maximum) {
            validated = false;
        }

        if (validated == false) {
            cout << endl <<"That is not a valid choice. Please enter a valid choice." << endl;
            cin >> input;
            cin.ignore();
        }

    } while (validated == false);

    return input;
}

//long int overload
long int validateInputInclusive(long int input, long int minimum, long int maximum) {
    bool validated = true;

    do {
        validated = true;

        //is the input even an int? used with aid from http://www.learncpp.com/cpp-tutorial/185-stream-states-and-input-validation/
        if (cin.fail())
            {
                validated = false;
                cin.ignore();
            }

        //is the int within the specified range?
        if (input < minimum || input > maximum) {
            validated = false;
        }

        if (validated == false) {
            cout << endl <<"That is not a valid choice. Please enter a valid choice." << endl;
            cin >> input;
            cin.ignore();
        }

    } while (validated == false);

    return input;
}

//double overload
double validateInputInclusive(double input, double minimum, double maximum) {
    bool validated = true;

    do {
        validated = true;

        //is the input even an int? used with aid from http://www.learncpp.com/cpp-tutorial/185-stream-states-and-input-validation/
        if (cin.fail())
            {
                validated = false;
                cin.ignore();
            }

        //is the int within the specified range?
        if (input < minimum || input > maximum) {
            validated = false;
        }

        if (validated == false) {
            cout << endl <<"That is not a valid choice. Please enter a valid choice." << endl;
            cin >> input;
            cin.ignore();
        }

    } while (validated == false);

    return input;
}

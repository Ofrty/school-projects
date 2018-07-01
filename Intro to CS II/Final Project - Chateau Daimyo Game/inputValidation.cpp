/*********************************************
**Program Name: inputValidation.cpp
**Author: Joe Kirkham
**Date: 2017/02/26
**Description: This is the implementation file for input validation functions.
**             The functions are to be used to ensure that any prompted input
**             is correctly entered by the user, helping to ensure the program
**             won't continue on without it.
**
**             IDEAS FOR LATER EXPANSION:
**             -convert everything from 'validateInputInclusive.hpp' to functions
**              in here that also handle the cin parts
*********************************************/

#include <iostream>
#include <limits>

using std::cin;
using std::cout;
using std::endl;
using std::numeric_limits;
using std::streamsize;

/*********************************************
**letterInput() checks to make sure that a user has entered a letter as indicated.
**It is case-insensitive, as it handles uppercase inputs, by always forcing lowercase.
**Note that whatever calls this must be expecting a lowercase return from letterInput().
*********************************************/
char letterInput() {
    char temp;
    bool validated = true;

    do {
        validated = false;

        //get the char from user
        cin.get(temp);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');    //with aid from http://www.cplusplus.com/forum/beginner/121194/
        //also, the above precludes possibility of 'a 7' as a way to quickly add a number via the menu, but I'm going by the post on:
        //https://piazza.com/class/iv5rux38y871ci?cid=579 which says that we don't have to

        //check to see if it's actually a letter
        if ( (temp < 65 || temp > 90) && (temp < 97 || temp > 122) ) {
            validated = false;
        } else {
            validated = true;
        }

        //if invalid, tell the user
        if (!validated) {
            cout << endl
                 << "**That is not a valid input. Please enter just a single character." << endl;
        }

        //clear any flags
        cin.clear();

    } while (!validated);

    //force lowercase
    temp = tolower(temp);

    return temp;
}

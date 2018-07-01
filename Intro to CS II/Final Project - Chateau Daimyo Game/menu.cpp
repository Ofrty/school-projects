/*********************************************
**Program Name: menu.cpp
**Author: Joe Kirkham
**Date: 2017/01/21
**Description: This is the implementation file for the function menu().
**             The purpose of this functions is provide a quick way to build
**             a menu. The function accepts two arguments: the number of choices
**             in the menu, and an array of strings that contains the text for the
**             options. It specifies a final option 0 to quit. The function also
**             handles the user's menu choice, and returns the integer choice to be
**             used by the calling function.
**
**             Also: this function makes use of the validateInputInclusive() function,
**             to validate integer input.
**
**             CHANGELOG:
**             *2017/01/29 (cruise update)        - Moved the prompting to re-enter if invalid to validateInputInclusive(),
**                                                  instead of in this menu function.
**             *2017/02/18 (fuck Belk update)     - Created a new option that does not allow for a quit
**                                                - Created custom program name overloads for the standard and noquit menus
**                                                - Created promptReturnMenu() that simply prompts for a return to the menu
**             *2017/02/19 (mouse day update)     - Created custom program name & *custom quit wording* overload (standard menu)
**             *2017/02/26 (Tati cookies update)  - Created versions of the menu that accept char inputs; added "inputValidation" to #includes
**             *2017/03/04 (two Ostbyes, one Lin) - Created version of the int quit&noquit menus that allows for all prev options + custom instructions
**             *2017/03/22 (wet shoe closet floor)- Modified the 'promptMenuReturns' to accept a second 'condition' argument
**             IDEAS FOR LATER EXPANSION:
**             -customized menu instructions argument
*********************************************/

#include <iostream>
#include <string>
#include "menu.hpp"
#include "validateInputInclusive.hpp"
#include "inputValidation.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::string;

/*********************************************
**menu presents a list of choices, as determined by the passed
**options[] argument. It also provides a quit option. There
**also an overload that allows for a custom program name.
*********************************************/
int menuInt(int numChoices, string options[]) {
    int userChoice;

    //print the menu
    cout << "***** Program Options *****" << endl;

    for (int i = 1; i <= numChoices; i++) {
        cout << i << ". " << options[i-1] << endl;
    }
    cout << "0. Quit" <<  endl << endl;

    //prompt for choice
    cout << "**Please enter the number of your choice above, or press 0 to quit." << endl;
    cin >> userChoice;
    cin.ignore();

    //validate choice
    userChoice = validateInputInclusive(userChoice,0,numChoices);

    return userChoice;
}

//overload for custom program name
int menuInt(int numChoices, string options[], string name) {
    int userChoice;

    //print the menu
    cout << "***** " << name << " *****" << endl;

    for (int i = 1; i <= numChoices; i++) {
        cout << i << ". " << options[i-1] << endl;
    }
    cout << "0. Quit" <<  endl << endl;

    //prompt for choice
    cout << "**Please enter the number of your choice above, or press 0 to quit." << endl;
    cin >> userChoice;
    cin.ignore();

    //validate choice
    userChoice = validateInputInclusive(userChoice,0,numChoices);

    return userChoice;
}

//overload for custom program name & custom quit wording
int menuInt(int numChoices, string options[], string name, string quit) {
    int userChoice;

    //print the menu
    cout << "***** " << name << " *****" << endl;

    for (int i = 1; i <= numChoices; i++) {
        cout << i << ". " << options[i-1] << endl;
    }
    cout << "0. " << quit << endl << endl;

    //prompt for choice
    cout << "**Please enter the number of your choice above, or press 0 to " << quit << "." << endl;
    cin >> userChoice;
    cin.ignore();

    //validate choice
    userChoice = validateInputInclusive(userChoice,0,numChoices);

    return userChoice;
}

//int selectors overload for custom program name, custom quit wording, & custom instructions
int menuInt(int numChoices, string options[], string name, string quit, string instructions) {
    int userChoice;

    //print the menu
    cout << "***** " << name << " *****" << endl;

    for (int i = 1; i <= numChoices; i++) {
        cout << i << ". " << options[i-1] << endl;
    }
    cout << "0. " << quit << endl << endl;

    //prompt for choice
    cout << "**" << instructions << ", or press 0 to " << quit << "." << endl;
    cin >> userChoice;
    cin.ignore();

    //validate choice
    userChoice = validateInputInclusive(userChoice,0,numChoices);

    return userChoice;
}

//basic char selectors
char menuChar(int numChoices,string options[]) {
    char userChoice;

    //print the menu
    cout << "***** Program Options *****" << endl;

    for (int i = 1; i <= numChoices && i < 25; i++) {
        cout << static_cast<char>(i+96) << ". " << options[i-1] << endl;
    }
    cout << "x. Quit" <<  endl << endl;

    //prompt for choice
    cout << "**Please enter the letter of your choice above, or enter x to quit." << endl;
    userChoice = letterInput();
    cin.ignore();

    return userChoice;
}

//char selectors overload for custom program name
char menuChar(int numChoices, string options[], string name) {
    char userChoice;

    //print the menu
    cout << "***** " << name << " *****" << endl;

    for (int i = 1; i <= numChoices && i < 25; i++) {
        cout << static_cast<char>(i+96) << ". " << options[i-1] << endl;
    }
    cout << "x. Quit" <<  endl << endl;

    //prompt for choice
    cout << "**Please enter the letter of your choice above, or enter x to quit." << endl;
    userChoice = letterInput();
    cin.ignore();

    return userChoice;
}
//char selectors overload for custom program name & custom quit wording
char menuChar(int numChoices, string options[], string name, string quit) {
    char userChoice;

    //print the menu
    cout << "***** " << name << " *****" << endl;

    for (int i = 1; i <= numChoices && i < 25; i++) {
        cout << static_cast<char>(i+96) << ". " << options[i-1] << endl;
    }
    cout << "x. " << quit << endl << endl;

    //prompt for choice
    cout << "**Please enter the letter of your choice above, or enter x to " << quit << "." << endl;
    userChoice = letterInput();
    cin.ignore();

    return userChoice;
}

/*********************************************
**menuNoQuit() works the same as menu above, but does not allow
**the user to quit. It also has a custom program name overload.
********************************************/
int menuIntNoQuit(int numChoices, string options[]) {
    int userChoice;

    //print the menu
    cout << endl
         << "***** Program Options *****" << endl;

    for (int i = 1; i <= numChoices; i++) {
        cout << i << ". " << options[i-1] << endl << endl;
    }

    //prompt for choice
    cout << "**Please enter the number of your choice above." << endl;
    cin >> userChoice;
    cin.ignore();

    //validate choice
    userChoice = validateInputInclusive(userChoice,1,numChoices);

    return userChoice;
}

//custom program name overload
int menuIntNoQuit(int numChoices, string options[],string name) {
    int userChoice;

    //print the menu
    cout << endl
         << "***** " << name << " *****" << endl;

    for (int i = 1; i <= numChoices; i++) {
        cout << i << ". " << options[i-1] << endl << endl;
    }

    //prompt for choice
    cout << "**Please enter the number of your choice above." << endl;
    cin >> userChoice;
    cin.ignore();

    //validate choice
    userChoice = validateInputInclusive(userChoice,1,numChoices);

    return userChoice;
}

//custom program name overload & custom instructions
int menuIntNoQuit(int numChoices, string options[],string name,string instructions) {
    int userChoice;

    //print the menu
    cout << endl
         << "***** " << name << " *****" << endl;

    for (int i = 1; i <= numChoices; i++) {
        cout << i << ". " << options[i-1] << endl << endl;
    }

    //prompt for choice
    cout << "**" << instructions << "." << endl;
    cin >> userChoice;
    cin.ignore();

    //validate choice
    userChoice = validateInputInclusive(userChoice,1,numChoices);

    return userChoice;
}

/*********************************************
**promptMenuReturn() simply prompts the user to hit enter to return to the menu.
**It takes in an argument that should be the last choice the user made in
**the menu. Logic: for int menus, since 0 is always quit in my menus,
**then this function won't prompt if 0 is the passed int.
********************************************/
//int input
void promptMenuReturn(int choice, int condition) {
    if (choice != condition) {
        cout << endl
             << "**Press Enter to return to the menu." << endl;
        cin.ignore();
    }
}

//char input
void promptMenuReturn(char choice, char condition) {
    if (choice != condition) {
        cout << endl
             << "**Press Enter to return to the menu." << endl;
        cin.ignore();
    }
}

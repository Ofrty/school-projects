/*********************************************
**Program Name: menu.hpp
**Author: Joe Kirkham
**Date: 2017/01/21
**Description: This is the header file for the menu() function.
**             (see the .cpp for more details)
*********************************************/

#ifndef MENU_HPP
#define MENU_HPP

#include <string>

using std::string;

/**Menu with quit options**/
//basic int selectors
int menuInt(int,string[]);
//int selectors overload for custom program name
int menuInt(int,string[],string);
//int selectors overload for custom program name & custom quit wording
int menuInt(int,string[],string,string);
//int selectors overload for custom program name, custom quit wording, & custom instructions
int menuInt(int,string[],string,string,string);

//basic char selectors
char menuChar(int,string[]);
//char selectors overload for custom program name
char menuChar(int,string[],string);
//char selectors overload for custom program name & custom quit wording
//char menuChar(int,string[],string,string);


/**Menu without quit option**/
int menuIntNoQuit(int,string[]);
//overload for custom program name
int menuIntNoQuit(int,string[],string);
//overload for custom program name & custom instructions
int menuIntNoQuit(int,string[],string,string);

/**Return to menu**/
//int input
void promptMenuReturn(int, int);
//char input
void promptMenuReturn(char, char);

#endif // MENU_HPP



/**********************************************
**Program Name: RicePaddy.cpp
**Author: Joe Kirkham
**Date: 2017/03/16
**Description: This is the implementation file for class Rice Paddy.
**             It's not very special; the only special action is to
**             take the clothes of a drunk guard. Otherwise, it's
**             just a normal space for a player to move through.
**********************************************/

#include <iostream>
#include <string>
#include "RicePaddy.hpp"
#include "joeUtilities.hpp"
#include "menu.hpp"

using std::string;
using std::cout;
using std::endl;

/**********************************************
**The Rice Paddy constructor simply sets the name of the space.
**********************************************/
RicePaddy::RicePaddy() {
    name = "Rice Paddy";
}

/**********************************************
**The RicePaddy destructor deletes all items from the entities list.
***********************************************/
RicePaddy::~RicePaddy() {
     while (!entities.empty()) {
        delete entities.back();
        entities.pop_back();
    }
}

/**********************************************
**fullDescription() generates a description of the Space, based on the current
**state of the Space. It describes the current space, and what's to the NESW.
**Rice Paddy fields may have a drunk guard singing in them, which may allow a special action.
**The p argument is not used in this class.
**********************************************/
void RicePaddy::fullDescription(Player* p) {
    cout << endl
         << "*******************************" << endl;
    joePrint("You are in a Rice Paddy.");

    //special stuff for this space
    //if there's a drunk guard, check and see if he's got his clothes on
    if (searchItems("Drunk Guard (Clothed)") != -1) {
        joePrint("There's a guard, drunkenly dancing and singing in the field. He doesn't care that you're there. He looks to be about the same size as you.");
    } else if (searchItems("Drunk Guard (Nude)") != -1) {
        joePrint("There's a guard, drunkenly dancing and singing in the field. He doesn't appear to care that he's nude.");
    }

    //report north
    if (north != NULL) {
        seeNorth();
    }

    //report east
    if (east != NULL) {
        seeEast();
    }

    //report south
    if (south != NULL) {
        seeSouth();
    }

    //report west
    if (west != NULL) {
        seeWest();
    }
}

/**********************************************
**actionMenu() will give a list of action options, dependent upon
**the status of entities in the space. Options 1-4 will be to go
**NESW (if possible), Option 5 will be to take an item or do an action.
**For a RicePaddy, the special action will be to take a guard's clothes - IF
**you haven't already. Player* p is not used here.
**********************************************/
int RicePaddy::actionMenu(Player* p) {
    int numChoices;

    //if clothed guard, give option to take clothes. else, just the directions.
    if (searchItems("Drunk Guard (Clothed)") != -1) {
        numChoices = 5;
        string choices[] = {"Go North",
                            "Go East",
                            "Go South",
                            "Go West",
                            "Take the guard's clothes"
                           };

        uChoice = menuIntNoQuit(numChoices, choices);
    } else {
        numChoices = 4;
        string choices[] = {"Go North",
                            "Go East",
                            "Go South",
                            "Go West",
                           };

        uChoice = menuIntNoQuit(numChoices, choices);
    }

    return uChoice;
}

/**********************************************
**specialAction() triggers a special action for this Space child.
**It is passed a Player* pointer so that it can directly modify
**a Player object, if necessary. For a RicePaddy, the special
**action is to take a guard's clothes, which it does by invoking
**the Player's changeClothing() method.
**********************************************/
void RicePaddy::specialAction(Player* p) {
    joePrint("\"O-sake wa dou?\", you say to the guard. He strips his clothing with a disturbing amount of finesse. You take it.");

    //change the player's outfit
    p->changeClothing("Guard Outfit");

    //change guard from clothed to nude
    int i = searchItems("Drunk Guard (Clothed)");
    delete entities[i];
    entities[i] = new Entity("Drunk Guard (Nude)");
}

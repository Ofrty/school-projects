/**********************************************
**Program Name: Wall.cpp
**Author: Joe Kirkham
**Date: 2017/03/16
**Description: This is the implementation file for class Wall.
**             Wall is derived from the Space class, and has
**             special actions 'scale', 'peek', and 'climb over'.
**             (though, 'Climb Over' is a fancy way of saying 'go north').
**********************************************/

#include <iostream>
#include <string>
#include "Wall.hpp"
#include "joeUtilities.hpp"
#include "menu.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::string;

/**********************************************
**The Wall constructor sets the name of the space, and sets the wall pos to bottom.
**********************************************/
Wall::Wall() {
    name = "Wall";
}

/**********************************************
**The Wall destructor deletes all items from the entities list.
***********************************************/
Wall::~Wall() {
     while (!entities.empty()) {
        delete entities.back();
        entities.pop_back();
    }
}

/**********************************************
**fullDescription() generates a description of the Space, based on the current
**state of the Space. It describes the current space, and what's to the NESW.
**The description for Wall depends on whether the player is at the bottom or the
**top of the Wall. The p argument is not used in this class
**********************************************/
void Wall::fullDescription(Player* p) {
    cout << endl
         << "*******************************" << endl;

    //special description for this space
    switch(pos) {
        case BOTTOM:
            joePrint("You are at the bottom of the chateau's outside wall.");
            joePrint("You think you could scale it, if you wanted to.");
        break;

        case TOP:
            joePrint("You are at the top of the chateu's outside wall.");
            joePrint("You can peek over the top, climb over the top, or climb down.");
        break;
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
**actionMenu() will give a list of actions, dependent upon
**the status of entities in the space. Options 1-4 will be to go
**NESW (if possible), Options greater than 5 will be to take an item or do an action.
**For a Wall, the special actions depend on whether the player starts at the bottom or
**the top. If at the bottom, the player can climb to the top. If at the top, the
**player can peek, climb down, or climb over. Player* p is not used here.
**Note also that if at the bottom, a player can't simply 'go north' over a wall; they can look up instead.
**********************************************/
int Wall::actionMenu(Player* p) {
    int uChoice;
    int numChoices;

    //if at bottom, can climb to top; can't go north, can only look up
    if (pos == BOTTOM) {
        do { //while (uChoice != 1);
            numChoices = 5;
            string choices[] = {"Look Up",
                                "Go East",
                                "Go South",
                                "Go West",
                                "Scale the wall"
                               };

            uChoice = menuIntNoQuit(numChoices, choices);

            //if they picked 1, just troll them
            if (uChoice == 1) {
                joePrint("Yep, that's a 'hai' wall, alright.");
                joePrint("Press Enter to show the actions again.");
                cin.get();
            }

        } while (uChoice == 1);
    } else {
        numChoices = 5;
        string choices[] = {"Climb Over (go north)",
                            "Climb Down and Go East",
                            "Climb Down and Go South",
                            "Climb Down and Go West",
                            "Peek Over"
                            };

        uChoice = menuIntNoQuit(numChoices, choices);
    }

    return uChoice;
}

/**********************************************
**specialAction() for a Wall is dependent on whether the player is at the top
**or the bottom of the Wall. If bottom, they have the option to climb. If Top,
**they have the option to peek.
**********************************************/
void Wall::specialAction(Player* p) {
    //if at the bottom, climb up to the top
    if (pos == BOTTOM) {
        joePrint("You scale the wall and stop just before the very top.");
        pos = TOP;
        p->setCurrentLocation(p->getCurrentLocation());
    } else { //if at the top, peek over
        if (getNorth()->getEntities().empty()) { //note: will crash if north of wall is nothing!
        joePrint("There's nobody in sight, just some empty ground and the chateau building.");
        } else {
        joePrint("There's a guard. He doesn't see you yet!");
        }
        p->setCurrentLocation(p->getCurrentLocation());
    }
}

/**********************************************
**setPos() is a special Wall 'setter' that will determine
**whether the position on the wall is top or bottom, depending
**on whether the passed value is a chateau ground or a rice paddy
**********************************************/
void Wall::setPos(Space* prev) {
    if (prev->getName() == "Rice Paddy") {
        pos = BOTTOM;
    } else if (prev->getName() == "Chateau Ground") {
        pos = TOP;
    }
}

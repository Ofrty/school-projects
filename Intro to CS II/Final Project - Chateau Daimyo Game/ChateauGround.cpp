/**********************************************
**Program Name: ChateauGround.cpp
**Author: Joe Kirkham
**Date: 2017/03/24
**Description: This is the implementation file for class ChateauGround.
**             It is similar to a ChateauGround, in that it's not that special.
**             It is possible to be caught by a guard here, but that's determined within
**             the ChateauDaimyo class.
**********************************************/


#include <iostream>
#include <string>
#include "ChateauGround.hpp"
#include "joeUtilities.hpp"
#include "menu.hpp"

using std::string;
using std::cout;
using std::endl;

/**********************************************
**The Chateau Ground constructor simply sets the name of the space, isCaught to false, and numVisits to 0.
**********************************************/
ChateauGround::ChateauGround() {
    name = "Chateau Ground";
    isCaught = false;
    numVisits = 0;
}

/**********************************************
**The ChateauGround destructor deletes all items from the entities list.
***********************************************/
ChateauGround::~ChateauGround() {
     while (!entities.empty()) {
        delete entities.back();
        entities.pop_back();
    }
}

//getter
bool ChateauGround::getIsCaught() {
    return isCaught;
}

/**********************************************
**fullDescription() generates a description of the Space, based on the current
**state of the Space. It describes the current space, and what's to the NESW.
**Chateau Grounds may have a guard in them, which doesn't cause a special action,
**but the guard will react differently depending on the player's clothing (which is detected through argument p)
**********************************************/
void ChateauGround::fullDescription(Player* p) {
    cout << endl
         << "*******************************" << endl;
    joePrint("You are on Chateau Grounds.");

    //increment numVisits
    numVisits += numVisits + 1;

    //special stuff for this space
    //if there's a guard, he'll react to you. if not, just nothing around you
    if (searchItems("Guard") != -1) {
        //if player is in a guard outfit, nothing happens, but if the player isn't, guard catches you!
        if (p->getClothing()->name == "Guard Outfit") {
            //first time,
            if (numVisits == 1) {
                joePrint("There's a guard who turns and notices you in your Guard Outfit as you walk into this area. \"Nomanai!\", he hisses at you in the dark, but otherwise leaves you alone. He seems a tad suspicious, though...");
            } else {
                isCaught = true;
                joePrint("The suspicious guard takes a harder look at you, and realizes you're not his drunk partner. \"KISAMA!\", he shouts, and levels his spear toward you!");
            }
        } else {
            isCaught = true;
            joePrint("There's a guard who turns and notices you in your Ninja Outfit as you walk into this area. He jumps to and shouts, \"KISAMA!\", and points his spear at you!");
        }
    } else {
        joePrint("You're on Chateau Grounds, and there's nothing immediately around you.");
    }

    if (!isCaught) {
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
}

/**********************************************
**actionMenu() will give a list of action options, dependent upon
**the status of entities in the space. Options 1-4 will be to go
**NESW (if possible), Option 5 will be to take an item or do an action.
**For a ChateauGround, there is no special action. Also, options 1-4
**will only display if the player hasn't been caught by a guard! P is not used here.
**********************************************/
int ChateauGround::actionMenu(Player* p) {
    if (!isCaught){
        int numChoices = 4;
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
**Since this space has no special action, nothing happens.
**********************************************/
void ChateauGround::specialAction(Player* p) {
    //do nothing
}

/**********************************************
**Program Name: DaimyoRoom.cpp
**Author: Joe Kirkham
**Date: 2017/03/24
**Description: This is the implementation file for class DaimyoRoom.
**             This is a derived class of Space, and serves as the
**             location where the Player must achieve a victory
**             condition by killing the Daimyo. The player can then
**             also take the Daimyo's jewel.
**********************************************/

#include <iostream>
#include <string>
#include "DaimyoRoom.hpp"
#include "joeUtilities.hpp"
#include "menu.hpp"

using std::string;
using std::cout;
using std::endl;

/**********************************************
**The DaimyoRoom constructor simply sets the name of the space and isDaimyoAlive to true.
**********************************************/
DaimyoRoom::DaimyoRoom() {
    name = "Daimyo Room";
    isDaimyoAlive = true;
}

/**********************************************
**The DaimyoRoom destructor deletes all items from the entities list.
***********************************************/
DaimyoRoom::~DaimyoRoom() {
     while (!entities.empty()) {
        delete entities.back();
        entities.pop_back();
    }
}

//getter
bool DaimyoRoom::getIsDaimyoAlive() {
    return isDaimyoAlive;
}

/**********************************************
**fullDescription() generates a description of the Space, based on the current
**state of the Space. It describes the current space, and what's to the NESW.
**DaimyoRoom will be described differently, depending on whether or not the Daimyo is alive.
**********************************************/
void DaimyoRoom::fullDescription(Player* p) {
    cout << endl
         << "*******************************" << endl;
    joePrint("You are in the Daimyo's Personal Chambers.");

    //special stuff for this space
    //if daimyo is alive
    if (isDaimyoAlive) {
        joePrint("Inside the Daimyo's private room, you see a robed figure kneeling in prayer. He is adorned with a jeweled necklace.");
    } else if (p->searchItems("Jewel") == -1) {
        joePrint("The Daimyo is slumped over, a shuriken sticking out of his neck.");
    } else {
        joePrint("The Daimyo is slumped over, a shuriken sticking out of his neck. You notice a hole the shape of the jewel in the west wall.");
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
**For a DaimyoRoom, the special action will be dependent upon whether the
**Daimyo is alive. If alive, you can throw a shuriken at him. If dead,
**you can take his jewel. p is used to check if player has jewel already.
**********************************************/
int DaimyoRoom::actionMenu(Player* p) {
    int numChoices;

    //if daimyo alive, give option to kill him
    if (isDaimyoAlive) {
        numChoices = 5;
        string choices[] = {"Go North",
                            "Go East",
                            "Go South",
                            "Go West",
                            "Throw your Shuriken at the figure's neck"
                           };

        uChoice = menuIntNoQuit(numChoices, choices);
    } else if (p->searchItems("Jewel") == -1) { //if he's dead and you don't have his jewel
        numChoices = 5;
        string choices[] = {"Go North",
                            "Go East",
                            "Go South",
                            "Go West",
                            "Take Jewel"
                           };

        uChoice = menuIntNoQuit(numChoices, choices);
    } else if (west == NULL) { //if he's dead and you do have his jewel
        numChoices = 5;
        string choices[] = {"Go North",
                            "Go East",
                            "Go South",
                            "Go West",
                            "Place Jewel In Wall"
                           };
        uChoice = menuIntNoQuit(numChoices, choices);
    } else {//if you've opened the wall
        numChoices = 4;
        string choices[] = {"Go North",
                            "Go East",
                            "Go South",
                            "Go West Through Secret Passage",
                           };
        uChoice = menuIntNoQuit(numChoices, choices);
    }

    return uChoice;
}

/**********************************************
**specialAction() triggers a special action for this Space child.
**For a Daimyo room: if the daimyo is alive,then you can kill him.
**If he's dead and you haven't taken his jewel, then you can take his
**jewel.
**********************************************/
void DaimyoRoom::specialAction(Player* p) {
    //if daimyo alive, give option to kill him
    if (isDaimyoAlive) {
        joePrint("You slide a Shuriken out from your belt. As you aim to throw, he looks up and catches your eye. You throw, and it hits him in the throat. He clutches, sags, and stops moving.");
        isDaimyoAlive = false;
    } else if (p->searchItems("Jewel") == -1) { //if he's dead and you don't have his jewel
        joePrint("You take the jewel from the Daimyo's necklace - this could be valuable!");
        p->addItem(removeItem(searchItems("Jewel")));
    } else {
        joePrint("You place the jewel in the wall, and a secret passage opens - this might help you avoid that guard out front! Luckily, you remove the jewel and can take it with you!");
        west = p->getCurrentLocation()->getSouth()->getWest();
    }
}


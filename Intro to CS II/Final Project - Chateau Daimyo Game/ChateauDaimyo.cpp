/**********************************************
**Program Name: ChateauDaimyo.cpp
**Author: Joe Kirkham
**Date: 2017/03/16
**Description: This is the implementation file for class Game.
**             Its purpose is store information and run methods
**             relevant for running the text-based game Chateau Daimyo.
**             It relies on calling methods of the Player class, and of child
**             Space classes.
**********************************************/

#include <iostream>
#include <string>
#include "ChateauDaimyo.hpp"
#include "joeUtilities.hpp"

using std::cin;
using std::cout;
using std::endl;

/**********************************************
**The Game constructor sets the stage for a game of CD, primarily by
**creating the Spaces and the Player
**********************************************/
ChateauDaimyo::ChateauDaimyo() {
    //set parameters
    moveCount = 0;
    choiceValid = false;
    hasLost = hasWon = false;

    //create spaces
    rp1 = new RicePaddy;
    rp2 = new RicePaddy;
    w1 = new Wall;
    w2 = new Wall;
    cg1 = new ChateauGround;
    cg2 = new ChateauGround;
    dr = new DaimyoRoom;

    //set space parameters
    rp1->setAllDirections(w1,rp2,NULL,NULL);
    rp1->addItem(new Entity("Drunk Guard (Clothed)"));
    rp2->setAllDirections(w2,NULL,NULL,rp1);
    w1->setAllDirections(cg1,w2,rp1,NULL);
    w2->setAllDirections(cg2,NULL,rp2,w1);
    cg1->setAllDirections(NULL,cg2,w1,NULL);
    cg2->setAllDirections(dr,NULL,w2,cg1);
    cg2->addItem(new Entity("Guard"));
    dr->setAllDirections(NULL,NULL,cg2,NULL);
    dr->addItem((new Entity("Jewel")));

    theNinja = new Player(rp2, "Ninja Outfit");
}

/**********************************************
**The Game destructor deletes the Game's Player and Spaces.
**********************************************/
ChateauDaimyo::~ChateauDaimyo() {
    //delete player
    delete theNinja;
    theNinja = NULL;

    //delete spaces
    delete rp1;
    rp1 = NULL;
    delete rp2;
    rp2 = NULL;
    delete w1;
    w1 = NULL;
    delete w2;
    w2 = NULL;
    delete cg1;
    cg1 = NULL;
    delete cg2;
    cg2 = NULL;
    delete dr;
    dr = NULL;
}

/**********************************************
**playGame() continuously calls for descriptions of the current
**location and prompts for actions. Depending on the user's choice,
**playGame() will either move the player or trigger the Space's special action.
**********************************************/
void ChateauDaimyo::playGame() {
    cout <<  "**You are an elite Ninja, garbed in an all-black outfit." << endl
         <<  "*Your master has tasked you with hunting down and assassinating a Daimyo in hiding. Hiding is dishonorable!" << endl
         <<  "*Your target has secluded himself in a French-style chateau, tucked up tightly in a mountainous rock formation." << endl
         <<  "*He keeps a couple of small rice paddies outside his chateau walls - you enter in one of them." << endl
         <<  "*You know his meager security is weakest and most lax just before the dawn - so the guard is weak, but time is short!" << endl << endl;

    joePrint("Press Enter to begin in the rice paddy.");
    cin.get();

    do { //while ((moveCount < MAX_MOVES) && !hasWon)
        choiceValid = false;

        //describe and show action menu for current spot
        theNinja->getCurrentLocation()->fullDescription(theNinja);
        actionChoice = theNinja->getCurrentLocation()->actionMenu(theNinja);

        if (moveCount == 13) {
            joePrint("The sky is turning more and more orange - you haven't much time!");
        }

        if (moveCount == 16) {
            joePrint("The sun is coming over the horizon! You have only seconds before the light will be too bright!");
        }

        switch(actionChoice) {
            //go north
            case 1:
                if (theNinja->getCurrentLocation()->getNorth() != NULL) {
                    choiceValid = true;
                    theNinja->setCurrentLocation(theNinja->getCurrentLocation()->getNorth());
                } else {
                    joePrint("There's nothing to the north! Make another choice.");
                }
            break;

            //go east
            case 2:
                if (theNinja->getCurrentLocation()->getEast() != NULL) {
                    choiceValid = true;
                    theNinja->setCurrentLocation(theNinja->getCurrentLocation()->getEast());
                } else {
                    joePrint("There's nothing to the east! Make another choice.");
                }
            break;

            //go south
            case 3:
                if (theNinja->getCurrentLocation()->getSouth() != NULL) {
                    choiceValid = true;
                    theNinja->setCurrentLocation(theNinja->getCurrentLocation()->getSouth());
                } else {
                    joePrint("There's nothing to the south! Make another choice.");
                }
            break;

            //go west
            case 4:
              if (theNinja->getCurrentLocation()->getWest() != NULL) {
                    choiceValid = true;
                    theNinja->setCurrentLocation(theNinja->getCurrentLocation()->getWest());
                } else {
                    joePrint("There's nothing to the west! Make another choice.");
                }
            break;

            //special action; will only appear if the space wills it!
            default:
                theNinja->getCurrentLocation()->specialAction(theNinja);
                choiceValid = true;
            break;
        }

        //if the next spot is a wall, determine the position on the wall (top or bottom)
        if (theNinja->getCurrentLocation()->getName() == "Wall") {
            theNinja->getCurrentLocation()->setPos(theNinja->getPrevLocation());
        }

        //check to see if player has won or if the player has lost
        checkWon();
        checkLost();

        //increment movecount IF the choice was valid; report
        if (choiceValid) {
            moveCount++;
            joePrint("Press Enter to see what happens next!");
            cin.get();
        } else {
            joePrint("Press Enter to show the information for this space again.");
            cin.get();
        }
    } while ((moveCount <= MAX_MOVES) && !hasWon && !hasLost); //make a function that changes hasWon based on location and whether the daimyo is dead

    //if the player has won, report it to the user!
    if (hasWon) {
        joePrint("Success! You've completed your mission, and can report back to your Master. The Daimyo no longer lives in dishonor. Also, now you have a fancy jewel!");
    }

    //if the player has lost, report to user!
    if (hasLost) {
        joePrint("You've been caught! Now your last decision will be: perform seppuku and die honorably, or to be a coward and die with dishonor!");
    }
}

/**********************************************
**hasWon() checks to see if the player has met both of the victory conditions:
** 1) daimyo dead
** 2) player has escaped back to a rice paddy
**********************************************/
void ChateauDaimyo::checkWon() {
    if ((theNinja->getCurrentLocation()->getName() == "Rice Paddy") && (dr->getIsDaimyoAlive() == false)) {
        hasWon = true;
    } else {
        hasWon = false;
    }
}

/**********************************************
**hasLost() checks to see if the player has met either of the losing conditions:
** 1) turn count run out
** 2) caught by a guard
**Also checks first to see if they've met the win condition first
**********************************************/
void ChateauDaimyo::checkLost() {
    if (!hasWon) {
        //if move count is up
        if ((moveCount == MAX_MOVES)) {
            joePrint("You're out of time - the light of the dawn reveals you!");
            hasLost = true;
        }
        //if caught by a guard while not disguised
        if (cg2->getIsCaught() == true) {
            hasLost = true;
        }
    } else {
        hasLost = false;
    }
}

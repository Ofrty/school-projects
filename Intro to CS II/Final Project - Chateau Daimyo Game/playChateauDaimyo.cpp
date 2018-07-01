//this is only a test file for player, test, and space (and derived spaces)

#include <iostream>
#include <string>
#include "ChateauDaimyo.hpp"
#include "Space.hpp"
#include "RicePaddy.hpp"
#include "Player.hpp"
#include "menu.hpp"
#include "validateInputInclusive.hpp"
#include "joeUtilities.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main() {
    ChateauDaimyo* theGame;

    int mainMenuChoice;
    string mainMenuOptions[] = {"Play",
                                "Show Instructions on How to Win"
                               };
    do { //while (mainMenuChoice != 0);
        mainMenuChoice = menuInt(2,mainMenuOptions,"Chateau Daimyo Launcher");

        switch (mainMenuChoice) {
            //play the game
            case 1:
                theGame = new ChateauDaimyo();
                theGame->playGame();
                joePrint("Press Enter to end the game");
                cin.get();
                delete theGame;
            break;

            case 2:
                joePrint("To Win: West, Take Clothes, East, North, Scale, North, North, Throw Shuriken, Take Jewel, Place in West Wall, West, South, Climb Down South");
            break;
        }
    } while (mainMenuChoice != 0);

    return 0;
}

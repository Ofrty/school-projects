/**********************************************
**Program Name: ChateauDaimyo.hpp
**Author: Joe Kirkham
**Date: 2017/03/23
**Description: This is the header file for class Game.
**             (see implementation file for more info)
**********************************************/

#ifndef CHATEAU_DAIMYO_HPP
#define CHATEAU_DAIMYO_HPP

#include <vector>
#include "Player.hpp"
#include "Space.hpp"
#include "RicePaddy.hpp"
#include "Wall.hpp"
#include "ChateauGround.hpp"
#include "DaimyoRoom.hpp"

class ChateauDaimyo {
    private:
        const int MAX_MOVES = 18;
        int moveCount;
        int actionChoice;
        bool choiceValid;
        bool hasWon;
        bool hasLost;
        Player* theNinja;
        Space* rp1;
        Space* rp2;
        Space* w1;
        Space* w2;
        Space* cg1;
        Space* cg2;
        Space* dr;

    public:
        //constructor
        ChateauDaimyo();

        //destructor
        ~ChateauDaimyo();

        //methods
        void playGame();
        void checkWon();
        void checkLost();
};

#endif // CHATEAU_DAIMYO

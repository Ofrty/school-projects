/**********************************************
**Program Name: ChateauGround.hpp
**Author: Joe Kirkham
**Date: 2017/03/24
**Description: This is the header file for class ChateauGround.
**             (see implementation file for details)
**********************************************/

#ifndef CHATEAU_GROUND_HPP
#define CHATEAU_GROUND_HPP

#include "Space.hpp"

class ChateauGround : public Space {
    private:
        bool isCaught;
        int numVisits;

    public:
        //constructor
        ChateauGround();

        //destructor
        ~ChateauGround();

        //getter
        virtual bool getIsCaught();

        //methods
        virtual void fullDescription(Player*) override;
        virtual int actionMenu(Player*) override;
        virtual void specialAction(Player*) override;
};

#endif // CHATEAU_GROUND_HPP

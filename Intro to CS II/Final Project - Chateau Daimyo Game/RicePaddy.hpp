/**********************************************
**Program Name: RicePaddy.hpp
**Author: Joe Kirkham
**Date: 2017/03/16
**Description: This is the header file for class RicePaddy.
**             (see implementation file for more info)
**********************************************/

#ifndef RICE_PADDY_HPP
#define RICE_PADDY_HPP

#include "Space.hpp"
#include "Player.hpp"

class RicePaddy : public Space {
    public:
        //constructor
        RicePaddy();

        //destructor
        ~RicePaddy();

        //methods
        virtual void fullDescription(Player*) override;
        virtual int actionMenu(Player*) override;
        virtual void specialAction(Player*) override;
};

#endif // RICE_PADDY_HPP

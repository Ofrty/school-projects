/**********************************************
**Program Name: DaimyoRoom.hpp
**Author: Joe Kirkham
**Date: 2017/03/24
**Description: This is the header file for class DaimyoRoom.
**             (see implementation file for details)
**********************************************/

#ifndef DAIMYO_ROOM_HPP
#define DAIMYO_ROOM_HPP

#include "Space.hpp"

class DaimyoRoom : public Space {
    private:
        bool isDaimyoAlive;

    public:
        //constructor
        DaimyoRoom();

        //destructor
        ~DaimyoRoom();

        //getter
        virtual bool getIsDaimyoAlive();

        //methods
        virtual void fullDescription(Player*) override;
        virtual int actionMenu(Player*) override;
        virtual void specialAction(Player*) override;
};

#endif // DAIMYO_ROOM_HPP

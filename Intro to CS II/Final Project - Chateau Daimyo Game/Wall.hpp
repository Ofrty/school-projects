/**********************************************
**Program Name: Wall.hpp
**Author: Joe Kirkham
**Date: 2017/03/22
**Description: This is the header file for class Wall.
**             (see implementation file for details)
**********************************************/

#ifndef WALL_HPP
#define WALL_HPP

#include "Space.hpp"

enum Position {BOTTOM, TOP};

class Wall : public Space {
    private:
        Position pos;

    public:
        //constructor
        Wall();

        //destructor
        ~Wall();

        //methods
        virtual void fullDescription(Player*);
        virtual int actionMenu(Player*) override;
        virtual void specialAction(Player*) override;
        void setPos(Space*);
};

#endif // WALL_HPP

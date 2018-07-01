/**********************************************
**Program Name: Space.hpp
**Author: Joe Kirkham
**Date: 2017/03/15
**Description: This is the header file for class Space.
**             (see implementation file for more info)
**********************************************/

#ifndef SPACE_HPP
#define SPACE_HPP

#include <string>
#include <vector>
#include "Player.hpp"

using std::string;
using std::vector;

struct Entity {
    string name;

    //constructor sets name and isRemovable
    Entity(string n) {
        name = n;
    }
};

class Player; //forward declare

class Space {
    protected:
        string name;
        Space* north;
        Space* east;
        Space* south;
        Space* west;
        vector<Entity*> entities;
        int uChoice;

    public:
        //destructor
        virtual ~Space();

        //getters
        virtual Space* getNorth();
        virtual Space* getEast();
        virtual Space* getSouth();
        virtual Space* getWest();
        virtual string getName();
        virtual vector<Entity*> getEntities();
        virtual bool getIsDaimyoAlive();
        virtual bool getIsCaught();

        //setters
        virtual void setAllDirections(Space*,Space*,Space*,Space*);
        virtual void setNorth(Space*);
        virtual void setEast(Space*);
        virtual void setSouth(Space*);
        virtual void setWest(Space*);

        //methods
        virtual int searchItems(string);
        virtual void addItem(Entity*);
        virtual Entity* removeItem(int);
        virtual void seeNorth();
        virtual void seeEast();
        virtual void seeSouth();
        virtual void seeWest();
        virtual void fullDescription(Player*) = 0; //abstract
        virtual int actionMenu(Player*) = 0; //abstract
        virtual void specialAction(Player*) = 0; //abstract
        virtual void setPos(Space*); //abstract
};

#endif // SPACE_HPP

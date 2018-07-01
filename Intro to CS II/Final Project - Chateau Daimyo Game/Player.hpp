/**********************************************
**Program Name: Player.hpp
**Author: Joe Kirkham
**Date: 2017/03/15
**Description: This is the header file for class Player.
**             (see implementation file for details)
**********************************************/

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>
#include "Space.hpp" //entity struct

using std::string;
using std::vector;

struct Entity; //forward declare
class Space; //forward declare

class Player {
    private:
        Space* currentLocation;
        Space* prevLocation;
        Entity* clothing;
        vector<Entity*> playerBag;

    public:
        //constructor
        Player(Space*, string);

        //destructor
        ~Player();

        //getters
        Entity* getClothing();
        Space* getCurrentLocation();
        Space* getPrevLocation();

        //setter
        void setCurrentLocation(Space*);

        //methods
        void changeClothing(string);
        int searchItems(string);
        void addItem(Entity*);
};

#endif // PLAYER_HPP

/**********************************************
**Program Name: Player.cpp
**Author: Joe Kirkham
**Date: 2017/03/20
**Description: This is the implementation file for class Player.
**             This class is meant to contain information about a
**             player moving through spaces in a text-based game,
**             including current location and items.
**********************************************/

#include <iostream>
#include <string>
#include "Player.hpp"
#include "Space.hpp"
#include "joeUtilities.hpp"

using std::string;

/**********************************************
**The Player constructor sets first argument to the Player's initial
**location, and sets its clothing entity's name to the second argument string.
**********************************************/
Player::Player(Space* s, string cl) {
    currentLocation = s;
    prevLocation = NULL;
    clothing = new Entity(cl);
}

//destructor
/**********************************************
**The Player destructor empties the bag and deletes the clothing.
**********************************************/
Player::~Player() {
    delete clothing;

    //if bag is not empty, delete the items in the bag
    while (!playerBag.empty()) {
        delete playerBag.back();
        playerBag.pop_back();
    }
}

//getters
Entity* Player::getClothing() {
    return clothing;
}

Space* Player::getCurrentLocation() {
    return currentLocation;
}

Space* Player::getPrevLocation() {
    return prevLocation;
}

//setter
void Player::setCurrentLocation(Space* cl) {
    //also set the prev location to current location
    prevLocation = currentLocation;

    currentLocation = cl;
}

/********************************************
**changeClothing() deletes current clothing and
**changes it to something new.
********************************************/
void Player::changeClothing(string c) {
    joePrint("Clothing changed from " + clothing->name + " to " + c);

    delete clothing;
    clothing = new Entity(c);
}

/********************************************
**searchItems() looks through the player's bag
**to see if there are any new items w/ same name as argument.
**If found, returns the index. If not found, returns -1
********************************************/
int Player::searchItems(string n) {
    for (int i = 0; i < playerBag.size(); i++) {
        if (playerBag[i]->name == n) {
            return i; //if found
        }
    }

    //if not found
    return -1;
}

/********************************************
**addItem() adds an entity to a player's bag.
********************************************/
void Player::addItem(Entity* i) {
    playerBag.push_back(i);
}

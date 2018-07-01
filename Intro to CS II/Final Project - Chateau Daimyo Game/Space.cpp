/**********************************************
**Program Name: Space.cpp
**Author: Joe Kirkham
**Date: 2017/03/16
**Description: This is the header file for class Space.
**             It is an abstract class that can be used
**             serve as the parent for creating basic
**             spaces for a character to move through and
**             interact with.
**********************************************/

#include "Space.hpp"
#include "joeUtilities.hpp"

/**********************************************
**The Space destructor serves only to exist to be inherited by its children.
**********************************************/
Space::~Space() {}

//getters
Space* Space::getNorth() {
    return north;
}

Space* Space::getEast() {
    return this->east;
}

Space* Space::getSouth() {
    return this->south;
}

Space* Space::getWest() {
    return this->west;
}

string Space::getName() {
    return this->name;
}

vector<Entity*> Space::getEntities() {
    return entities;
}

bool Space::getIsDaimyoAlive() {
    //does nothing here, only used by DaimyoRoom
    return true;
}

bool Space::getIsCaught() {
    //does nothing here Chateau Ground
    return false;
}

//setters
void Space::setAllDirections(Space* n, Space* e, Space* s, Space* w) {
    north = n;
    east = e;
    south = s;
    west = w;
}

void Space::setNorth(Space* s) {
    north = s;
}

void Space::setEast(Space* s) {
    east = s;
}

void Space::setSouth(Space* s) {
    south = s;
}

void Space::setWest(Space* s) {
    west = s;
}

//methods
/**********************************************
**searchItems() will look for a particular item
**in the Space's Entities list. If found, it will
**return the index of the item in the Entities list.
**If not, it returns -1.
**********************************************/
int Space::searchItems(string s) {
    //look for it
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i]->name == s) {
            //if found, return index
            return i;
        }
    }

    //if not found
    return -1;
}

/**********************************************
**addItem() will 'add' an item to a space's entities list
**********************************************/
void Space::addItem(Entity* i) {
    entities.push_back(i);
}

/**********************************************
**removeItem() will 'remove' an Item from a Space's
**entities list by setting its pointer to 'NULL' and
**placing the item as the tempTransfer
**********************************************/
Entity* Space::removeItem(int index) {
    Entity* temp = entities[index];
    entities[index] = NULL;
    return temp;
}

/**********************************************
**seeNorth/East/South/West will all each return the name
**of the location to the North/East/South/West of the current Space.
**********************************************/
void Space::seeNorth(){
    joePrint("To the north is a " + north->getName());
}

void Space::seeEast(){
    joePrint("To the east is a " + east->getName());
}

void Space::seeSouth(){
    joePrint("To the south is a " + south->getName());
}

void Space::seeWest(){
    joePrint("To the west is a " + west->getName());
}

/**********************************************
**setPost() is just a dummy implementation, only to be really used by a wall
**********************************************/
void Space::setPos(Space* s) {
    //do nothing
}

/*************************************************
**Filename: Map.cpp
**Authors: Jadin, Kim, Kirkham
**Date Created: 2017/08/15
**Date Modified: 2017/08/18
**Description:
**  Map is a class that contains information about a particular
**  graph in Euclidean space that contains Cities (nodes), that each
**  have an ID, an x-coordinate, and a y-coordinate (stored in the City struct).
**  The Map class is designed to be used for solving TSP problems. Tours are stored
**  and defined in a Tour struct, which contain information about the stops along
**  the Tour.
**
**  The primary algorithms for solving TSP inputs are greedy() and twoOpt().
**  Please see the function headers in the implementation file Map.cpp for a
**  full description of these, and other, Map class methods.
*************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits> //max int
#include <cmath> //sqrt & pow

using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;
using std::vector;

#ifndef MAP_HPP
#define MAP_HPP

#define nc 16000
#define GRID_MAX 500

struct City
{
    int id;
    int idx;
    int x;
    int y;
};

struct Edge
{
    int length;
    City cityA;
    City cityB;
};

struct Tour
{
    vector<int> stops;
    int length = 0;
    vector<Edge> legs;
};

class Map
{
public:
    Map();
    ~Map();

    //getters
    int getTotalDist();
    int getCount();

    //methods
    bool readTourFromFile(string);
    void outputTourToFile(string);
    void setDistGrid();
    int findDistance(City a, City b);
    int tourDist(Tour);
    void kirkhamBuild();
    void greedy();
    void twoOpt();
    void printGrid();
    void printLegs();

private:
    int count;
    City cities[nc];
    int dist[GRID_MAX][GRID_MAX];
    int totalDist;
    Tour T;
};

#endif


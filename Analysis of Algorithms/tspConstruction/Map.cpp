/*************************************************
**Filename: Map.cpp
**Authors: Jadin, Kim, Kirkham
**Date Created: 2017/08/15
**Date Modified: 2017/08/18
**Description:
**  See the header file Map.hpp for a full description of
**  the Map class, City struct, and Tour struct declared there.
**  See function headers below for descriptions of methods in the
**  Map class.
*************************************************/

#include <array>
#include "Map.hpp"

using std::vector;

Map::Map()
{
    count = 0;
    totalDist = 0;
    T.length = 0;
}

Map::~Map()
{
    cout << "\nPraise the moon!\n";
}

/*getters*/
int Map::getTotalDist()
{
    return totalDist;
}

int Map::getCount()
{
    return count;
}

/********************************************************
**readTourFromFile reads information on nodes ('cities') in a Euclidean space.
**The cities are stored in a City struct that holds information on the city's
**index in the input, its x coordinate, and y coordinate. All cities are then
**stored in a Tour struct that contains information about the cities visited
**along the tour, and a basic tour is constructed on the order that cities are
**read from file.
**
**PARAMETERS: A filename assumed to have the following format:
**Line A - index of city
**Line B - x coordinate
**Line C - y coordinate
**  repeat ABC for every city in the input
**
**RETURNS: Tour struct, consisting of a Tour of the cities as they were read
**from file.
********************************************************/
bool Map::readTourFromFile(string inFileName)
{
    /*
    ifstream inFile;
    City tempCity;
    int tempIDX;

    //open file
    inFile.open(inFileName);

    if (!(inFile.fail())) //ensure file opened
    {
        cout << "**Input file successfully opened." << endl;

        int n = 0;
        //import cities and create tour w/ length as brought in
        while (inFile >> tempIDX) //first int is id
        {
            //create temp city and store in tour
            tempCity.id = tempIDX;
            tempCity.idx = n;
            inFile >> tempCity.x; //second int is x
            inFile >> tempCity.y; //third int is y
            cities[n] = tempCity;

            n++;
        }

        count = T.length = n;

        inFile.close();

        cout << "**Tour read and stored. " << n << " cities in tour." << endl;
    }
    else
    {
        cout << "**The input file was not found." << endl;
        return false;
    }

    return true;
    */
    ifstream inFile;
    City tempCity;
    int tempIDX;
    int tempDist = 0;

    //open file
    inFile.open(inFileName);

    if (!(inFile.fail())) //ensure file opened
    {
        cout << "**Input file successfully opened." << endl;

        int n = 0;
        //import cities and create tour w/ length as brought in
        while (inFile >> tempIDX) //first int is id
        {
            //create temp city and store in tour
            tempCity.id = tempIDX;
            tempCity.idx = n;
            inFile >> tempCity.x; //second int is x
            inFile >> tempCity.y; //third int is y
            T.stops.push_back(n);
            cities[n] = tempCity;

            //calculate distance from previous to next, add to total length
            if (n > 0) //don't calculate distance for first city
            {
                tempDist = findDistance(cities[tempIDX], cities[tempIDX-1]);
            }

            totalDist += tempDist;
            //tempTour.tourLegs.push_back(tempDist); //legs if we want them!

            n++;
        }

        //add on the distance from the last city back to the first city
        tempDist = findDistance(cities[n-1], cities[0]);

        count = T.length = n;
        totalDist += tempDist;

        inFile.close();

        cout << "**Tour read and stored. " << n << " cities in tour." << endl;
    }
    else
    {
        cout << "**The input file was not found." << endl;
        return false;
    }

    return true;
}

/********************************************************
**outputTourToFile will write the contents of a Tour to a file named in the 2nd parameter.
**The first line will be the length of the tour, the following lines will be the input index
**of tours visited.
**
**PARAMETERS: tour, output file name
********************************************************/
void Map::outputTourToFile(string s)
{
    ofstream outFile;

    outFile.open(s);

    //output tour Length
    outFile << totalDist << '\n';

    //loop through all items in now-sorted vector
    for (int i = 0; i < count; i++)
    {
        outFile << T.stops[i] << '\n';
    }

    outFile.close();

    cout << "**Tour exported to file " << s << "." << endl;
}

/********************************************************
**setDistGrid creates a grid of distances between cities. Note
**that it is limited to only as high as defined by GRID_MAX
**(i.e., max size is GRID_MAX^2, since it's a 2D square grid).
**This helps to store some distance values for quick lookup when
**calculating distances. Values not stored here can be calculated
**on-demand.
********************************************************/
void Map::setDistGrid()
{
    for (int i = 0; (i < count) && (i < GRID_MAX); i++)
    {
        for (int j = i; j < count && (j < GRID_MAX); j++)
        {
            dist[i][j] = dist[j][i] = findDistance(cities[i], cities[j]);
        }
    }
}

/********************************************************
**findDistance calculates the distance between two cities, grabbing their
** x- and y- coordinates and putting them through the distance formula.
**
**PARAMETERS: two cities
********************************************************/
int Map::findDistance(City a, City b)
{
    return int(sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2)) + 0.5);
}

/********************************************************
**tourDist() calculates the entire length of a given tour. If distance between
**two cities is stored in the dist matrix, it will select from there.
**If not, it will calculate it on-demand.
********************************************************/
int Map::tourDist(Tour t)
{
    int sum = 0;
    int last = t.length - 1;

    for (int i = 0; i < last; i++)
    {
        //use the grid if both cities are within GRID_MAX
        if ((t.stops[i] < GRID_MAX) && (t.stops[i + 1] < GRID_MAX))
        {
            sum += dist[t.stops[i]][t.stops[i + 1]];

        } else //hand-calculate the distance if outside of the grid
        {
            sum += findDistance(cities[t.stops[i]], cities[t.stops[i + 1]]);
        }
    }

    //get last distance
    sum += findDistance(cities[t.stops[last]],cities[t.stops[0]]);

    return sum;
}

void Map::kirkhamBuild()
{
    cout << "**Executing kirkhamBuild algorithm" << endl;

    Tour tempTour;
    Edge tempLeg;
    int tempNewDist, minDist, tempAToNew, tempNewToB, candidateAToNew, candidateNewToB;
    int tempTotalDist = 0;
    int candidateLegIdx;

    //select two cities to start
    tempTour.stops.push_back(cities[0].id);
    tempTour.stops.push_back(cities[1].id);

    //build a leg between them
    tempLeg.length = findDistance(cities[tempTour.stops[0]],cities[tempTour.stops[1]]);
    tempLeg.cityA = cities[tempTour.stops[0]];
    tempLeg.cityB = cities[tempTour.stops[1]];

    tempTour.legs.push_back(tempLeg);

    //put a leg from the end back to the beginning
    tempLeg.length = findDistance(cities[tempTour.stops[1]],cities[tempTour.stops[0]]);
    tempLeg.cityA = cities[tempTour.stops[1]];
    tempLeg.cityB = cities[tempTour.stops[0]];

    tempTour.legs.push_back(tempLeg);

    //set the length
    tempTotalDist = tempTour.legs[0].length + tempTour.legs[1].length;

    //loop through all cities
    for (int v = 2; v < count; v++)
    {
        //int tempLegCount = (int)tempTour.legs.size();
        minDist = std::numeric_limits<int>::max();
        Edge newLeg;

        //loop through all the legs
        for (int e = 0; e < (int)tempTour.legs.size(); e++)
        {

            //check how long the tour would be by breaking the current leg e to go to the new city v
            tempAToNew = findDistance(tempTour.legs[e].cityA, cities[v]);
            tempNewToB = findDistance(cities[v], tempTour.legs[e].cityB);
            tempNewDist = tempAToNew + tempNewToB - tempTour.legs[e].length;

            //if tempNewDist < minDist, make it the new candidate
            if (tempNewDist < minDist)
            {
                minDist = tempNewDist;
                candidateLegIdx = e;
                candidateAToNew = tempAToNew;
                candidateNewToB = tempNewToB;
            }
        }

        /*after looping through all legs, make the candidate the new leg and update the tour distance*/
        //create a new edge that goes from the new city to city v
        newLeg.length = candidateNewToB;
        newLeg.cityA = cities[v];
        newLeg.cityB = tempTour.legs[candidateLegIdx].cityB;

        //make the old leg go from cityA go to the new city v
        tempTour.legs[candidateLegIdx].length = candidateAToNew;
        tempTour.legs[candidateLegIdx].cityB = cities[v];

        //insert the new edge from new city v to cityB
        tempTour.legs.insert(tempTour.legs.begin() + candidateLegIdx, newLeg);

        //emplace citv v in stops
        tempTour.stops.insert(tempTour.stops.begin() + (candidateLegIdx + 1), cities[v].id);

        //update the tour distance
        tempTotalDist += minDist;
    }

    //set the tour
    T = tempTour;
    T.length = count;
    totalDist = tempTotalDist;
/*
    int legSum = 0;

    for (int i = 0; i < (int)tempTour.legs.size(); i++)
    {
        legSum += tempTour.legs[i].length;
    }

    cout << "calculated distance of legs is " << legSum << endl;
*/
}

/********************************************************
**greedy() is an algorithm that utilizes a nearest-neighbor, greedy
**heuristic for finding a tour through given cities. It will modify the
**Tour and totalDist members of the the Map object.
********************************************************/
void Map::greedy()
{
    cout << "**Executing greedy algorithm" << endl;

    int visitCount = 0;
    int minDist, tempDist;
    int tempTotalDist = 0;
    City curCity, nextCity;
    Tour tempTour = T;

    //set your start city
    curCity = cities[0];

    //create a table to check if it's been visited or not
    bool isVisited[count];
    for (int i = 0; i < count; i++)
    {
        isVisited[i] = false;
    }

    while (visitCount < count)
    {
        minDist = std::numeric_limits<int>::max();

        // loop through all cities
        for (int i = 0; i < count; i++)
        {

            //make sure we're not comparing against the same city
            if (curCity.idx != i)
            {
                // if current city is not in the list of visited check the distance and update minDist
                if (!isVisited[i])
                {
                    //find distance
                    //tempDist = findDistance(curCity, cities[i]); //doesn't use grid

                    //use the grid if both cities are within GRID_MAX
                    if ((curCity.idx < GRID_MAX) && ((i) < GRID_MAX))
                    {
                        tempDist = dist[curCity.idx][i];
                    } else //hand-calculate the distance if outside of the grid
                    {
                        tempDist = findDistance(curCity,cities[i]);
                    }

                    //update this to the 'greedy' choice if it's the best choice (so far)
                    if (tempDist < minDist)
                    {
                        minDist = tempDist;
                        nextCity = cities[i];
                    }
                }
            }
        }

        //go to the next city
        //add the distance
        if (minDist < std::numeric_limits<int>::max())
        {
            tempTotalDist += minDist;
        }

        //update tour and visit logs before we leave the current city
        tempTour.stops[visitCount] = curCity.id;
        isVisited[curCity.idx] = true;
        visitCount++;

        //move on to the next city
        curCity = nextCity;
    }

    //update the tour + last route
    totalDist =  tempTotalDist + findDistance(cities[tempTour.stops[visitCount-1]],cities[tempTour.stops[0]]);
    T = tempTour;

    cout << "**Greedy algorithm found a tour of length " << tempTotalDist << endl;
}

/********************************************************
**twoOpt() is an algorithm that finds the best possible 'swap' to uncross
**two crossing routes in order to generate a shorter tour. It will continue
**to do this until it can no longer improve. twoOpt modifies the Tour and totalDist
**members
********************************************************/
void Map::twoOpt()
{
    cout << "**Executing 2-OPT algorithm" << endl;

    int g1;
    Tour Tp = T;
    do {
        g1 = 0;
        int dist2 = totalDist;
        for (int i = 0; i < count; i++)
        {
            for (int j = i + 1; j < count; j++)
            {
                for (int k = 0; k < i; k++)
                {
                    Tp.stops[k] = T.stops[k];
                }

                for (int k = 0; k + i <= j; k++)
                {
                    Tp.stops[i + k] = T.stops[j - k];
                }

                for (int k = j + 1; k < count; k++)
                {
                    Tp.stops[k] = T.stops[k];
                }

                dist2 = tourDist(Tp);
                if (dist2 < totalDist)
                {
                    T = Tp;
                    totalDist = dist2;
                    g1++;
                }
            }
        }
    } while (g1 > 0);
}

void Map::printGrid()
{
    ofstream outFile;

    outFile.open("grid.txt");

    //output tour Length
    for (int i = 0; i < 76; i++)
    {
        for (int j = 0; j < 76; j++)
        {
            outFile << dist[i][j] << " ";
        }
        outFile << '\n';
    }

    outFile.close();
}

void Map::printLegs()
{
    ofstream outFile;

    outFile.open("legs.txt");
    for (int i = 0; i <= T.legs.size(); i++)
    {
        outFile << T.legs[i].length << '\n';
    }

}

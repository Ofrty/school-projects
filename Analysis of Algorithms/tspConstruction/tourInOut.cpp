/*************************************************
**Filename: tourInOut.cpp
**Authors: Jadin, Kim, Kirkham
**Date Created: 2017/08/15
**Date Modified: 2017/08/18
**Description:
**  tourInOut contains a main function for executing approximation heuristics
**  on instances of TSP. A TSP input will be read in and run through a greedy
**  (nearest neighbor) algorithm and a 2-OPT algorithm. Users may choose which
**  of these algorithms to use on the input (or, they may use both) by selecting
**  a, b, or c from the menu as prompted.
**
**  INPUT:
**  This program accepts a single command line argument that should specify
**  the name of a file that contains a list of city x- and y-coordinates. The
**  file should have the following format:
**
**  Line A - index of city
**  Line B - x coordinate
**  Line C - y coordinate
**  repeat ABC for every city in the input
**
**  Note that the argument must also include the extension, e.g., "tsp_example_1.txt".
**
**  OUTPUT:
**  The program will write the final optimized tour to a file, with the tour length
**  on the first line and the cities' IDs in order of their visits on the tour.
**  The file will be the same as the user-entered input filename, with ".tour" appended
**  to the end.
*************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <cmath> //sqrt & pow
#include <chrono>
#include "Map.hpp"
#include "inputValidation.hpp"

using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
    Map m;

    string inFileName(argv[1]);
    string outFileName = inFileName + ".tour";
    bool isFileSuccess;
    char uChoice;

    //read in from cmd line arg
    isFileSuccess = m.readTourFromFile(inFileName);

    if(isFileSuccess)
    {
        //temp
        m.setDistGrid();

        cout << "**Please select a choice from below: "
             << endl << "a. KirkhamBuild Only"
             << endl << "b. Greedy Only"
             << endl << "c. 2-OPT Only"
             << endl << "d. Both: KirkhamBuild, then 2-OPT"
             << endl << "e. Both: Greedy, then 2-OPT"
             << endl
             << endl << "Enter only a single, lower-case letter."
             << endl;

        uChoice = letterInput();

        //start recording time
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

        //kirkhamBuild
        if ((uChoice == 'a') || (uChoice == 'd'))
        {
            m.kirkhamBuild();
        }

        //greedy
        if ((uChoice == 'b') || (uChoice == 'e'))
        {
            m.greedy();
        }

        //optimize with twoOpt
        if((uChoice == 'c') || (uChoice == 'd') || (uChoice == 'e'))
        {
            m.twoOpt();
        }

        //output Tour
        m.outputTourToFile(outFileName);

        //report algo time
        std::chrono::steady_clock::time_point finish = std::chrono::steady_clock::now();
        std::chrono::milliseconds elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
        cout << endl << "**Tour completed in " << (elapsed.count()) << " milliseconds (" << (elapsed.count()/1000) << " seconds), with a Tour length of " << m.getTotalDist() << "." << endl;

        //m.printGrid();
        //m.printLegs();
    }

    return 0;
}

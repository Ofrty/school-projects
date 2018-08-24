# tspConstruction



Context:

The final project of my Analysis of Algorithms class was to implement three Traveling Salesman Problem algorithms.



Description:

We were provided with several text files (in subfolder 'tours/') that contain x/y coordinates in Euclidean space for nodes/cities. We were asked to write algorithms that would find efficient tours of these cities. From files 1-7, the number of cities increases incrementally from 50 (tour-input-1) to 5000 (tour-input-7).

The kirkhamBuild algorithm is the implementation of an algorithm that I thought of on my own, but it turned out to be a simple, well-known Insertion heuristic.

To run:
Option A) Using a make-compatible command prompt, simply run 'make' and run the executable 'tourInOut', and provide arg1 = 'tour-input-x.txt', where "x" is in [1,7]. The tour files are stored in the 'tours/' subfolder
Option B) Manually link and compile all .hpp and .cpp files in this folder, then run your executable and provide the argument as described in Option A.

Postmortem:

# ChateauDaimyo



Context:

Chateau Daimyo is a small text-based game written in C++ to fulfill my final project in Intro to CS II.



Description:

You are a ninja tasked with assasinating a daimyo living in a French-style chateau. It is near dawn - you must enter the chateau, assaninate the daimyo, and escape, all while avoiding detection. You must also race to beat the clock - the sun is rising!



To run:
Option A) Using a make-compatible command prompt, simply run 'make' and run the executable 'playChateauDaimyo'
Option B) Manually link and compile all .hpp and .cpp files in this folder, then run your executable.

Postmortem:
Autopsy performed ~1.5 years after project completion.

Generally happy with this project. Most objects, attributes, and methods are is modularized in a logical, efficient manner. I was able to follow the code structure fairly easily based on the comments.

Some oft-referenced attributes should have been declared as local variables for readability (e.g., in ChateauDaimyo.cpp, see the large number of times theNinja->getCurrentLocation() was used).

I can also see how I shoe-horned in a couple of bits of code at the end of the development process. E.g., Player.hpp has a function getClothing() that is a bit too specific to be on the Player object. There is another instance in the game where the player must acquire an item, whose controlling function lives within the *space* itself (DaimyoRoom.cpp). The inconsistency between item-acquiring functions is not very clean, and certainly not easily scalable.
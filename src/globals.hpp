#ifndef GLOBALS_HPP_INCLUDED
#define GLOBALS_HPP_INCLUDED

#include <fstream>
// Global Constants
namespace globals
{
    // Number of labs in current system
    const int NUMLABS=8;
    // Max possible length of user ID
    const int MAXIDLENGTH=5;
    // Number of Computers in Each Lab (NOTE: one need only change this constant to add extra computer stations to match added lab simulations
    const int LABSIZES[NUMLABS]={19,15,24,33,61,17,55,37};

    const int NUMUNIVERSITIES=NUMLABS; // 1 lab per university, so constant integer is set to NUMLABS

    const string UNIVERSITYNAMES[NUMUNIVERSITIES]={"The University of Michigan", "The University of Pittsburgh", "Stanford University",
    "Arizona State University", "North Texas State University", "The University of Alabama, Huntsville", "Princeton University",
    "Duquesne University"};  // Initialize the array of university names

}


#endif // GLOBALS_HPP_INCLUDED

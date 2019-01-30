#ifndef FILEWRITE_HPP_INCLUDED
#define FILEWRITE_HPP_INCLUDED

#include "ToString.hpp"
#include "globals.hpp"
#include <fstream>
#include <sstream>
#include <string>

using std::string;
using std::setw;
using std::setfill;
using std::ofstream;
using std::fstream;
using globals::MAXIDLENGTH;

extern fstream timeFile; // Used globally for use here and in main

void fileWrite(UserInfo object, char action)
{  // Function takes UserInfo object and an action, which is I or O depending whether user is logging in or logging out
   // I write all the user's information to the file and include a timestamp
    timeFile.clear();
    string ID=to_string(object.getID());
    int IDlength=ID.length();
    if (IDlength<MAXIDLENGTH)
    {
        timeFile << action << "- ID: " << setw(MAXIDLENGTH-IDlength) << setfill('0') << '0' << object.getID()
        << ", Name: " << object.getName() << ", Time Expenditure: " << object.getTime() << " minutes\n\t" << "Date: ";
    }
    else
    {
        timeFile << action << "- ID: " << object.getID() << ", Name: " << object.getName() << ", Time Expenditure: "
        << object.getTime() << " minutes\n\t" << "Date: ";
    }
    time_t now=time(0);
    char* timeOf=ctime(&now);
    timeOf[20]='/0';
    for (int k=0; k<19; k++)
    {
        timeFile << timeOf[k];
    }
    timeFile << "\n";
}


#endif // FILEWRITE_HPP_INCLUDED

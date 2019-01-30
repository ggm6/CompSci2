#ifndef USERINFO_HPP_INCLUDED
#define USERINFO_HPP_INCLUDED

#include <iostream>

using std::string;

class UserInfo
{
    public:
        UserInfo();  // Default constructor, this is the only one that is actually used in my program
        UserInfo(int a, string b, int c);  // If the program were to manually set ID, time, and name all at once, this would be called
        void setUserInfo(int a, string b, int c) {ID=a, name=b, time=c;} // If the second constructor were to be used, this function is called to set the private variables
        int getID() {return ID;}  // Returns ID when called
        string getName() {return name;}  // Returns name when called
        int getTime() {return time;}  // Returns time when called
        int getIndex() {return stationIndex;}  // Returns index when called
        void setIndex(int a) {stationIndex=a;}  // Sets station number object
        void setLab(int b) {labIndex=b;}  // Sets lab number for users on the waiting list
        int getLab() {return labIndex;}  // Returns lab number of users on waiting list
        void swap(UserInfo& x);  // Swap switches all of the information of two UserInfo objects
        bool operator != (int x); // Allows comparison of class object to integer
        bool operator == (int x); // Allows comparison of class object to integer
        void operator = (int x); // Overload allows class object to set its integer variable to the integer that function is given
        void operator = (string y);  // Overload allows class object to set its string variable to the string that function is given
    private:
        int ID;  // Private variables used
        string name;
        int time;
        int stationIndex;
        int labIndex;
};


#endif // USERINFO_HPP_INCLUDED

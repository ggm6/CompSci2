// Name: Garrett McDonnell
// ggm6@zips.uakron.edu
// Purpose: Program simulates user log in and out of different computer stations in different labs

/* NOTES: Future versions may use strtod/atof in order to convert a string to a double, which could be cast to an int if needed,
          which would be far more efficient in order to bypass my costly validations.  I was highly annoyed in discovering this
          function exists in the lab.
          PROGRAM IS DYNAMIC, NODES ARE CREATED/DELETED UPON LOGIN/LOGOUT
*/


#include <iostream>       // I add these includes to use cin and cout
#include <iomanip>      // Allows for manipulations of input and output, such as setw and setfill
#include <ctime>        // Allows best use of rand and srand by seeding a random number by using a complex algorithm using time
#include <cstdlib>      // Allows use of rand and srand
#include <cstring>      // Used for turning string into char array
#include <string>       // Used for string manipulation
#include <cmath>  // For use of power function
#include <sstream> // For to_string function
#include <fstream>  // To write to a file
#include <list> // For representation of queue, but allows iteration

#include "ToString.hpp"  // For to_string
#include "LinkList.hpp"  // Allows use of my linked list header file
#include "globals.hpp"
#include "FileWrite.hpp"  // Allows me to write to a file

using std::cout;      // For use of the necessary namespaces
using std::cin;
using std::endl;
using std::string;
using std::setw;
using std::setfill;
using std::list;
using globals::LABSIZES;
using globals::MAXIDLENGTH;
using globals::NUMLABS;
using globals::NUMUNIVERSITIES;
using globals::UNIVERSITYNAMES;

std::fstream timeFile("CS210ProjectDTimeFile.txt",std::ios::out | std::ios::in | std::ios::trunc); // fstream object in FileWrite.hpp allows me to use it in this file at start and end of main

void menu();
// Precondition: none
// Postcondition: Spits out menu of options for user
void startup();
// Precondition: none
// Postcondition: Displays welcome message with the 8 labs and their respective university names
int validate(string& s,int minNum,int maxNum);
// Precondition: Takes string sent into function from outside variable
// Postcondition: Turns string into integer that is within range of minNum and maxNum for return
void validateName(string& s, int maxLength);
// Precondition: takes string variable for name of user
// Postcondition: Makes sure name contains only letters, spaces and apostrophes may only be used between letters
int IDvalidate(string& s);
// Precondition: takes string variable for ID
// Postcondition: Turns string into integer that cannot be larger than maxNum for return
int validateTime(string& timeLog);

int main()
{  // I open text file with fstream variable in unnamed namespace, so logins/logouts are written for entire length of program
    LinkList lists[NUMLABS];  // Declare LinkList array of 8 linked lists, representing 1 for each lab
    list<UserInfo> waitList; // THIS IS THE STL LIST, USED TO SIMULATE THE QUEUE FOR LOGIN REQUESTS
    startup(); // Displays welcome message and each lab with its respective name
    int option;
    while (option!=6)   // There are 5 options, this loop executes until user presses 5 to cut the loop and quit program
    {
        option=0;
        string option2="";
        menu();         // Menu is displayed for all 5 options
        while (option<1 || option>6)
        {
            cout << "Your choice: ";
            getline(cin,option2);  // Getline is called to receive anything the user presses for option2
            option=validate(option2,1,6);  // validation requires the string be turned into an integer within parameters 1 and 6 and be returned to "option"
        }
        if (option==1)
        {
            login(lists,waitList);     // If user chooses option 1, login function executes
        }
// OPT #1 - Version C:
//          if (option==2) ... if (option==3)... if (option==4)... if (option==5)
//          This efficiency change was made, because, now, the program will no longer continue checking statements once "option"
//          satisfies one of the statements.  Therefore, the program is NANOSECONDS faster.
//          Version D:
        else if (option==2)
        {
            logout(lists,waitList);    // If user chooses option 2, function to logout executes
        }
        else if (option==3)
        {
            searchArr(lists,waitList);  // If user chooses option 3, search function executes to look for an ID
        }
        else if (option==4)
        {
            display(lists);    // If user chooses option 4, display function executes with option to display 1 lab or all labs
        }
        else if (option==5)
        {
            timeFile.clear();  // Buffer must be cleared to use seekp
            timeFile.seekp(0, std::ios_base::beg);  // seekp is using to get program to read from start of file
            string bigString;
            while (timeFile)
            {
                string smallString;
                getline(timeFile,smallString);
                if (bigString.find("\n\t"))
                {
                    bigString=bigString+smallString+"\n";
                }
                else
                    bigString=bigString+smallString;
            }
            cout << endl << bigString << endl;
        }
    }
    char action='O';
    for (int i=0; i<NUMLABS; i++)
    {  // I loop through and write to the file to log everybody out that was logged in as program comes to an end
        // I also call destructor to get rid of each linked list as we move through the for-loop
        LinkList::ListNode* nodePtr=lists[i].head;
        while (nodePtr)
        {
            fileWrite(nodePtr->object,action);
            nodePtr=nodePtr->next;
        }
        lists[i].~LinkList();
        delete nodePtr;
    }
    timeFile.close();  // Finally, I close the txt file at end of program
    return 0;
}

void menu()
{   // Menu function uses setw and setfill for aesthetic reasons in menu, my corporation name is displayed with the program name, options are then displayed
    cout << setw(80) << setfill('-') << '-' << endl;
    cout << setw(50) << setfill(' ') << "Garrett McDonnell Incorporated" << endl;
    cout << setw(44) << setfill(' ') << "Computer Lab System" << endl;
    cout << setw(80) << setfill('_') << '_' << endl;
    cout << setw(40) << setfill(' ') << "MAIN MENU" << endl;
    cout << "1) Simulate login" << endl << "2) Simulate Logoff" << endl << "3) Search" << endl << "4) Display a lab" << endl << "5) Print Log" << endl << "6) Quit" << endl;
    cout << setw(80) << setfill('_') << '_' << endl;
}

void startup()
{   // setw and setfill used to display spaces before welcome message, then loop displays each lab and university name
    cout << setw(50) << setfill(' ') << "Welcome, here is the list of available labs" << endl;
    for (int i=0; i<NUMUNIVERSITIES; i++)
    {
        cout << "Lab " << i+1 << " at " << UNIVERSITYNAMES[i] << endl;
    }
    cout << endl;
    cout << "Press ENTER to continue: ";
    string x="";
    getline(cin,x);
}

int validate(string& option, int minNum, int maxNum) // ERROR HANDLING FUNCTION #1
{   // validate takes whatever user gives as a string, I first turn the string into a char array, then the first loop simply
    // creates an integer by multiplying each element of the char array by a power of 10, based on the location of the element in the array
    // For example, if the user inputs "100", choice = 1*10^2 + 0*10^1 + 0*10^0
    long int choice=0;
    try {
    cout << endl;
    char charc[option.length()];
    strcpy(charc,option.c_str());
    double j=pow(10,(option.length()-1));
    for (unsigned int k=0; k<option.length(); k++)
    {
        choice+=(charc[k]-48)*j;
        j=j/10;
    }
    // This next loop runs through every element of the char array, and makes sure the user did not just press enter
    // The loop checks to make sure each element is a digit and that digit is within the range of minNum and maxNum
    // Otherwise, the error is displayed forcing the user to fix the input until it is correct
    int i=0;
        while (charc[i] || charc[0]==0)
        {
            if (!isdigit(charc[i]) || choice<minNum || choice>maxNum)
            {
// OPT #2 - Version C:
//                strcpy(charc,option.c_str());
//                j=pow(10,(option.length()-1));
//                choice=0;
//                for (unsigned int k=0; k<option.length(); k++)
//                {
//                    choice+=(charc[k]-48)*j;  // valid user's choice from the char array requires us to subtract 48 to get the correct number
//                    j=j/10;
//                }
//                cout << endl;
//                i=0;
//          This change was made, because we needed exception handling.  However, it is also an optimization change.  Many
//          operations in my validation are cut, including a loop, by using a while-loop (with many fewer operations)
//          where I call this validation.
//          Version D:
                throw "Error, you entered an illegal character.";
            }
            else
                i++;
        }
    }
    catch (char const* msg) {
    cout << msg << endl << endl;
    }
    return choice; // If validation is successful, the integer choice is returned
}

int IDvalidate(string& option) // ERROR HANDLING FUNCTION #2
{  // IDvalidate works much the same as validate, but now there is no minimum number, as my code would automatically display
   // my error message, considering a negative sign is not a digit.  However, I now hard-code the char array because IDs will
   // never be greater than 5 digits, and my code displays an error by default if user has string longer than 5 characters
    long int ID=0;
    try {
    cout << endl;
    char maxID[MAXIDLENGTH]={1};
    for (int i=0; i<MAXIDLENGTH; i++)
    {
        maxID[i]='1';
    }
    double j=pow(10,(MAXIDLENGTH-1));
    long int maxNum=0;
    for (int i=0; i<MAXIDLENGTH; i++)
    {
        int x=maxID[i]-48;
        x=x*9;
        char y=x+48;
        maxID[i]=y;
        maxNum+=(maxID[i]-48)*j;
        j=j/10;
    }
    j=pow(10,(option.length()-1));
    for (unsigned int k=0; k<option.length(); k++)
    {
        char y=option[k];
        int x=y-48;
        ID+=x*j;
        j=j/10;
    }
    // I use the same strategy as before to create the integer for the ID, the user still cannot start by pressing enter.
    // I loop through the entire char array, if the string was less than 5 characters, not every element of the char array
    // will hold a digit, and therefore the first condition of the if-statement catches it, the second and third conditions make
    // sure the user is not entering a string longer than 5 characters or entering a space anywhere in the string
    unsigned int i=0;
        while (i<option.length() || option[0]==0)
        {
            if (option.length()!=5 || !isdigit(option[i]) || ID>maxNum|| isspace(option[i]))
            {
                if (option.length()!=5) // Exception handling allowed for better optimization here, as well, but I did not use it as one of my five.
                    throw static_cast<int> (option.length()); // throwing a string's length() member is naturally a size_t, so I cast it as an int
                else
                    throw "Error, you entered an illegal character.";  // Exception message thrown as char const*
            }
            else
                i++;  // i is set to 0 again if condition is met, otherwise it is incremented to continue checking every element of char array
        }
    }
    catch (char const* msg) { // If user specified an ID that is not full of integers, this catches the exception
        cout << msg << endl << endl;
    }
    catch (int IDChoiceLength) { // This catches the exception if the length of chosen ID is not five
        cout << "Error, the ID you entered was off by " << abs(IDChoiceLength-MAXIDLENGTH) << " character(s) from 5.  " << endl << endl;
    }
    return ID; // Once the string passes all tests, the integer form is returned
}

void validateName(string& s, int maxLength)
{
    // Similar to other validations, I create a char array from the user input string, the user cannot start by pressing enter
    // if any character the user inputs is anything but a letter, an error is displayed, name cannot be longer than maxLength
    // Note: I do not allow user to enter apostrophes or spaces at the beginning or the end of the string
    cout << endl;
    int i=0;
    char nameChar[s.length()];
    strcpy(nameChar,s.c_str());
    int len=s.length();
    while ((i<len) || nameChar[0]==0)
    {
        if ((!isalpha(nameChar[i]) && nameChar[i]!='\'' && nameChar[i]!=' ') || (len>maxLength) || nameChar[0]==' ' || nameChar[len-1]==' ' || nameChar[0]=='\'' || nameChar[len-1]=='\'')
        {
            cout << "Error, name must be between 1 and 35 letters, please try again: ";
            getline(cin,s);
            strcpy(nameChar,s.c_str());
            len=s.length();
            i=0;
            cout << endl;
        }
        else
            i++;
    }
}

int validateTime(string& timeLog)
{
    cout << endl;
    int i=0;  // I request time for user spent at lab, user cannot press enter to start, nor use spaces, string must match 15,30,45, or 60
    char timeChar[timeLog.length()];
    strcpy(timeChar,timeLog.c_str());
    int c=pow(10,(timeLog.length()-1));
    int timeNum=0;
    for (int k=0; k<2; k++)
    {
        timeNum+=(timeChar[k]-48)*c;
        c=c/10;
    }
    int k=timeLog.length();
    while (i<k || timeChar[0]==0)
    {
        if ((timeLog!="15" && timeLog!="30" && timeLog!="45" && timeLog!="60") || isspace(timeChar[i]))
        {
            cout << "Error, time spent at station can only be 15, 30, 45, or 60 minutes, please try again: ";
            getline(cin,timeLog);
            strcpy(timeChar,timeLog.c_str());
            c=pow(10,(timeLog.length()-1));
            timeNum=0;
            for (int k=0; k<2; k++)
            {
                timeNum+=(timeChar[k]-48)*c;
                c=c/10;
            }
            i=0;
            cout << endl;
        }
        else
            i++;
    }
    return timeNum;
}

void login(LinkList arr[], list<UserInfo>& waitList)
{   // First, user is asked for lab number and station number, which are validated using my validation function
    string station, timeLog, labStr;
    int labNum=-1;
    while (labNum<1 || labNum > NUMLABS)
    {
        cout << "Enter the lab number the user is logging in from: ";
        getline(cin, labStr);
        labNum=validate(labStr,1,NUMLABS);
    }
    int ID;
    string shouldDisp;
    string name;
    int tally=0;
    LinkList::ListNode* nodePtr;  // To move through the list
    // Position nodePtr at the head of whichever list the user chose
    nodePtr = arr[labNum-1].head;
    // While nodePtr points to a node, traverse
    // the list.
    while (nodePtr)
    {  // I increment a tally for each lab seat that is not empty in that lab
       tally++;
       nodePtr = nodePtr->next;
    }
    nodePtr=arr[labNum-1].head;  // The nodePtr must be reset after each loop, to point it back to the first node of that list
    srand(time(0));
    long int j=pow(10,MAXIDLENGTH);
    ID={rand()%j}; // I use rand to get a random number between 0 and 99999
    int i=0;
    while (i<NUMLABS)
    {  // This structure loops through every node of every list to make sure the newly generated ID doesn't match another user's ID
        nodePtr=arr[i].head;
        while (nodePtr)
        {
            if (ID==nodePtr->object.getID() || ID==15 || ID==30 || ID==45 || ID==60)
            {  // If the ID were to match another user's ID, we must get a new ID, reset nodePtr to the head of the first list,
               // and begin checking to see if the new ID matches any of the IDs from any lab's list
                ID={rand()%j};
                nodePtr=arr[0].head;
                i=0;
            }
            else
                nodePtr=nodePtr->next;
        }
        i++;
    }
    nodePtr=arr[labNum-1].head;
    i=0;
    UserInfo *x;
    x=new UserInfo; // Finally, once we've reached this point, the user's information can finally begin being set
    *x=ID; // I start by setting the user's ID with the integer ID we just randomly generated using an overload
// OPT #3 - Version C:
//            nodePtr=arr[labNum-1].head;
//            while (nodePtr)
//            {  // I loop through that lab's list to find the correct node matching the user's chosen station, and output the user's ID
//               if (nodePtr->object.getIndex()==stationNum-1)
//               {
//                  string x=to_string(ID);
//                  int IDlength=x.length();
//                  int y=nodePtr->object.getID();
//                  if (IDlength<MAXIDLENGTH)
//                  {
//                      cout << "User ID: " << setw(MAXIDLENGTH-IDlength) << setfill('0') << '0' << y << endl << endl;
//                  }
//                  else
//                      cout << "User ID: " << y << endl << endl;
//                  break;  // Break is used, because once we find the right node and display it's ID, we know longer need to traverse nodes
//               }
//               nodePtr=nodePtr->next;
//            }
//          Version change, as displaying the ID the below way is more efficient.  We no longer have to loop through to find a node
//          Version D:
    if (tally==LABSIZES[labNum-1])
    {  // If tally ends up being the same as the number of computers of that particular lab, that means the lab is full
        cout << "Sorry, lab " << labNum << " at " << UNIVERSITYNAMES[labNum-1] <<
        " has reached capacity.  You will be placed on a wait list for this lab." << endl << endl;
        x->setLab(labNum-1);
    }
    else
    {
        int stationNum=0;
        while (stationNum<1 || stationNum>LABSIZES[labNum-1])
        {
            cout << "Enter computer station number the user is logging in to: ";
            getline(cin,station);     // I then request station number and make sure it is not less than 1 or greater than the max stations for the user-specified lab
            stationNum=validate(station,1,LABSIZES[labNum-1]);
        }
        while (nodePtr)
        {  // I loop through the nodes until the node's index matches the user's station choice.
           // Technically, the second part of the "&&" statement should be redundant, as nodes are only
           // created with a real ID, and deleted upon logoff, but it's here for continuity's sake
            if (nodePtr->object.getIndex()==stationNum-1)
            {
                stationNum=0;
                cout << "Error, this lab station is not empty." << endl << endl;
                while (stationNum<1 || stationNum>LABSIZES[labNum-1])
                {
                    cout << "Please reenter computer station number the user is logging in to: ";
                    getline(cin,station);
                    stationNum=validate(station,1,LABSIZES[labNum-1]);
                }
                nodePtr=arr[labNum-1].head;  // We must reset nodePtr to the head, to make sure user doesn't pick another taken station earlier in the list
            }
            else
                nodePtr=nodePtr->next;
        }
        x->setIndex(stationNum-1);  // Index is set one less than the station user choice, so that my indexes can start at 0
    }
    string z=to_string(ID);
    int IDlength=z.length();
    if (IDlength<MAXIDLENGTH)
    {
        cout << "User ID: " << setw(MAXIDLENGTH-IDlength) << setfill('0') << '0' << ID << endl << endl;
    }
    else
        cout << "User ID: " << ID << endl << endl;
    cout << "Enter name of user logging into lab station: "; // I request name and validate for that using validateName
    getline(cin,name);
    validateName(name,35);
// OPT #4 - Version C:
//          while (nodePtr)
//          {  // I again loop through to find the node of the user's chosen station, I then set the name using an overload
//              if (nodePtr->object.getID()==ID)
//              {
//                  nodePtr->object=name;
//                  break;
//              }
//              nodePtr=nodePtr->next;
//          }
//          nodePtr=arr[labNum-1].head;
//          Version change is due to, again, the fact that it is unnecessary to loop through the nodes to set members of a
//          UserInfo object.  I continue to set members of the object, and I will loop through at the end to set the node.
//          Version D:
    *x=name;
    cout << "Enter minutes spent at lab station (15/30/45/60): ";
    getline(cin,timeLog);
    int timeNum=validateTime(timeLog);
//    OPT #5 - Version C:
//             while (nodePtr)
//             {
//                  if (nodePtr->object.getID()==ID)
//                  {
//                      nodePtr->object=timeNum;
//                      fileWrite(nodePtr->object, action);
//                  }
//                  nodePtr=nodePtr->next;
//              }
//              Again, it is unnecessary to loop through to find the node and then set the node's object's information, as well
//              as write to the file.  It is far more efficient to set all the object's information, and then place the object
//              in a node at the end.
    *x=timeNum;
    char action='I';
    if (tally!=LABSIZES[labNum-1])
    {
        arr[labNum-1].insertNode(*x); // Insert node takes care of looping through to put the new UserInfo object into that lab's list
        fileWrite(*x,action);
        cout << "Login successful!" << endl << endl; // I mention that login is successful
    }
    else
        waitList.push_front(*x); // I use the STL list's push_front, because we are implementing it as a queue (First In First Out structure)
    cout << "Returning to main menu, press ENTER to continue: "; // Message to show that user is being returned to main menu
    z=""; // I use an arbitrary string with getline to simulate a "pause"
    getline(cin,z);
}

void display(LinkList arr[])
{
    string labStr;
    LinkList::ListNode *nodePtr;
    int labNum=0;
    while (labNum<1 || labNum>NUMLABS)
    {
        cout << "Which lab would you like to display?  ";
        getline(cin,labStr);      // User is asked which lab he/she wants to see displayed, the lab number is output along with all its stations
        labNum=validate(labStr,1,NUMLABS);
    }
    nodePtr=arr[labNum-1].head;
    cout << "LAB STATUS" << endl << "Lab #" << labNum << " at " << UNIVERSITYNAMES[labNum-1] << endl;
    for (int i=0; i<LABSIZES[labNum-1]; i++)
    {  // I request the lab from user and validate, int i runs from 0 to the number of stations of a particular lab
       //I then use this next structure to output a user's ID for when a node already exists with an index to match i
        if (nodePtr!=nullptr && nodePtr->object.getIndex()==i)
        {
            string ID=to_string(nodePtr->object.getID());
            int IDlength=ID.length();
            if (IDlength<MAXIDLENGTH && (i+1)<10)
            { // I use this to add 0s when an ID length is less than the max (5 currently). A space is added if the station number is single-digit
                cout << " " << i+1 << ": " << setw(MAXIDLENGTH-IDlength) << setfill('0') << '0'
                << nodePtr->object.getID() << endl;
            }
            else if (IDlength<MAXIDLENGTH && (i+1)>9)
            {  // Again, zeros added to make ID's length appear to be max ID length, but no space is added for double-digit station numbers
                cout << i+1 << ": " << setw(MAXIDLENGTH-IDlength) << setfill('0') << '0'
                << nodePtr->object.getID() << endl;
            }
            else if (IDlength==MAXIDLENGTH && (i+1)<10)
            {  // No zeros are added for ID lengths that are the max, but space is still added for single digit station number
                cout << " " << i+1 << ": " << nodePtr->object.getID() << endl;
            }
            else // No zeros added nor spaces for formatting
                cout << i+1 << ": " << nodePtr->object.getID() << endl;
            nodePtr=nodePtr->next;
        }
        else
        { // The else-statement is for when no node has been created for a station number, meaning nobody has logged into that station
            if ((i+1)>9)
            { // double digit station number
                cout << i+1 << ": Empty " << endl;
            }
            else
            { // single digit station number
                cout << " " << i+1 << ": Empty " << endl;
            }
        }
    }
}

void logout(LinkList arr[], list<UserInfo>& waitList) //DYNAMIC - NODES DELETED UPON LOGOUT, NODES CREATED FOR USERS LEAVING QUEUE
{   // I loop through every object of every linked list, sum increments with whatever is held in each position
    // NOTE: given these 8 labs with these numbers of stations, sum stays 0 if no user is yet logged in
    list<UserInfo>::iterator q=waitList.begin();
    while (q!=waitList.end())
    {
        cout << q->getID() << endl;
        ++q;
    }
    LinkList::ListNode *nodePtr;
    int sum=0;
    for (int i=0; i<NUMLABS; i++)
    {
        nodePtr=arr[i].head;
        while (nodePtr)
        {
            sum++;
            nodePtr=nodePtr->next;
        }
    }
    string IDstr;
    int tally=0,labNum=-1,stationNum=0, ID=-1;
    while ((ID<0 || ID>=pow(10,MAXIDLENGTH)) || IDstr.length()!=5)
    {
        cout << "What is the ID of the user you wish to logout?  ";
        getline(cin,IDstr);
        ID=IDvalidate(IDstr);   // User is asked for ID number, which is validated for MAXIDLENGTH digits
    }
    char action='O';  // action is O here for logouts
    int x=0;
    while (x<NUMLABS)
    { // I loop through every node of every lab's linked list, if the user's specified ID matches a node's ID, I log user out
        nodePtr=arr[x].head;
        stationNum=0;
        while (nodePtr)
        {
            if (nodePtr->object.getID()==ID)
            {
                labNum=x;
                fileWrite(nodePtr->object, action);  // I write to the file showing that the user has logged out
                arr[x].deleteNode(nodePtr->object);  // I then delete the node from memory
                cout << "Logout successful!" << endl;
                x=NUMLABS+1;
                break;
            }
            else
            { // Tally increments each time the ID is not found in one of the nodes
                tally++;
            }
            stationNum++;
            nodePtr=nodePtr->next;
        }
        ++x;
    }
    IDstr=to_string(ID);
    int IDlength=IDstr.length();
    if (tally==sum && IDlength==MAXIDLENGTH)
    {   // If tally results in being the same as the sum, then that means none of the lists' nodes hold the user's specified ID
        cout << "User " << ID << " is not yet logged in anywhere." << endl;  // If ID length is MAXIDLENGTH, output is normal
    }
    if (tally==sum && IDlength<MAXIDLENGTH)
    {
        cout << "User " << setw(MAXIDLENGTH-IDlength) << setfill('0') << '0' << ID << " is not yet logged in anywhere." << endl;
    }   // If ID length is less than MAXIDLENGTH, I make it appear to have MAXIDLENGTH digits using 0s
    tally=waitList.size();
    list<UserInfo>::iterator a=waitList.end();
    --a;  auto c=waitList.begin();
    auto b=waitList.end();
    --b;
    while (tally>0) // Loop through the entire waiting list, if the list contains one or more users
    {
        if (a->getLab()==labNum)  // If user's chosen lab matches the lab that was just logged out of, we will pop that user off the back
        {
            action='I';
            if (waitList.size()>1) // Executes if waiting list size is greater than 1
            {
                c=a; // I swap user info, and shift the one leaving the queue to the end of the queue
                auto d=c; d++;  // I realize this may not be the most efficient structure, but I was having a lot of trouble with the swap
                while (d!=waitList.end())
                {
                    c->swap(*d);
                    ++c;  ++d;
                }
                b->setIndex(stationNum); // User's index for their station in that particular lab is set based on the station that was just logged out of
                IDstr=to_string(b->getID());
                IDlength=IDstr.length(); // Output purposes
                arr[labNum].insertNode(*b); // I have to make a new node for user in queue, although, future versions could simply never delete the node upon logout to begin with, and just change user information
                fileWrite(*b,action);  // I write to the file, as user is removed from queue and logged into system
                if (IDlength==MAXIDLENGTH)
                    cout << endl << "User " << b->getID() << " is taken off the wait list and is now logging in!" << endl;
                else
                {
                    cout << endl << "User " << setw(MAXIDLENGTH-IDlength) << setfill('0') << '0' << b->getID() <<
                    " is taken off the wait list and is now logging in!" << endl;
                }

            }
            else
            { // Executes if only one user is on the waiting list, not necessary to perform a swap
                a->setIndex(stationNum);
                IDstr=to_string(a->getID());
                IDlength=IDstr.length();
                arr[labNum].insertNode(*a);
                fileWrite(*a,action);
                if (IDlength==MAXIDLENGTH)
                    cout << endl << "User " << a->getID() << " is taken off the wait list and is now logging in!" << endl;
                else
                {
                    cout << endl << "User " << setw(MAXIDLENGTH-IDlength) << setfill('0') << '0' << a->getID() <<
                    " is taken off the wait list and is now logging in!" << endl;
                }
            }
            waitList.pop_back(); // pop_back is used, because the last element on the list will have been in the "queue" the longest
            break;
        }
        --a; --tally;
    }
    cout << endl;
    cout << "Returning to main menu, press ENTER to continue: ";
    IDstr=""; // I again make an arbitrary string and use getline to simulate pausing the program
    getline(cin,IDstr);
}

void searchArr(LinkList arr[], list<UserInfo>& waitList)
{   // Like before, I set a sum that is equal to the ID member of every object in the array
    LinkList::ListNode *nodePtr;
    int sum=0,ID=-1;
    for (int i=0; i<NUMLABS; i++)
    {
        nodePtr=arr[i].head;
        while (nodePtr)
        {
            sum++;
            nodePtr=nodePtr->next;
        }
    }
    string IDstr;
    int tally=0;
    while ((ID<0 || ID>=pow(10,MAXIDLENGTH)) || IDstr.length()!=5)
    {
        cout << "What is the ID of the user you wish to find?  ";
        getline(cin,IDstr);
        ID=IDvalidate(IDstr);   // User is asked for ID number, which is validated for MAXIDLENGTH digits
    }
    IDstr=to_string(ID);  // For good measure. . .
    int IDlength=IDstr.length();
    int x=0;
    while (x<NUMLABS)
    {
        nodePtr=arr[x].head;
        while (nodePtr)
        { // I loop through every node of every list, I display all user's information, IDlength used for adding 0s to IDs like before
            if (nodePtr->object.getID()==ID && IDlength<MAXIDLENGTH)
            {
                cout << "User " << setw(MAXIDLENGTH-IDlength) << setfill('0') << '0' <<
                nodePtr->object.getID() << ", " << nodePtr->object.getName() << ", " << "is in lab " << x+1 <<
                " at computer " << nodePtr->object.getIndex()+1 << " for " << nodePtr->object.getTime() << " mins""." << endl << endl;
                cout << "Returning to main menu, press ENTER to continue: ";
                IDstr="";
                getline(cin,IDstr);
                x=NUMLABS+1;
                break;
            }
            else if (nodePtr->object.getID()==ID && IDlength==MAXIDLENGTH)
            {  // If the user's specified ID matched the ID held in object, the name, ID, and time are displayed
                cout << "User " << ID << ", " << nodePtr->object.getName() << ", " << "is in lab " << x+1  << " at computer "
                << nodePtr->object.getIndex()+1 << " for " << nodePtr->object.getTime() << " mins"<< "." << endl << endl;
                cout << "Returning to main menu, press ENTER to continue: ";
                IDstr="";
                getline(cin,IDstr);
                x=NUMLABS+1;
                break;
            }
            else
                tally++; // I increment tally for every node that does not contain the user's specified ID
            nodePtr=nodePtr->next;
        }
        ++x;
    }
    if (tally==sum)     // If tally does end up matching the sum, this means no node in any list contained the user's specified ID
    {  // I then display to the user that his/her specified ID is not yet logged in anywhere
        if (IDlength==MAXIDLENGTH)
        {
            cout << "User " << ID << " is not yet logged in anywhere." << endl << endl;
            cout << "Returning to main menu, press ENTER to continue: ";
            IDstr="";
            getline(cin,IDstr);
        }
        else
        {
            cout << "User " << setw(MAXIDLENGTH-IDlength) << setfill('0') << '0' << ID << " is not yet logged in anywhere." << endl << endl;
            cout << "Returning to main menu, press ENTER to continue: ";
            IDstr="";
            getline(cin,IDstr);
        }
    }
}

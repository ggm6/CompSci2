#include "LinkList.hpp"
#include "ToString.hpp"
#include "globals.hpp"
#include <iomanip>
#include <sstream>

using globals::MAXIDLENGTH;

UserInfo::UserInfo() : ID(-1), name(""), time(0), stationIndex(0), labIndex(0)
{
    // Constructor for UserInfo object
}

UserInfo::UserInfo(int a, string b, int c)
{
    setUserInfo(a,b,c); // This constructor is not used, but if it were, a separate function would be called to set the private variables based on whatever is given
}

bool UserInfo::operator != (int x)
{ // Overload is used to allow comparison between the object's integer private members and the integer it's given
    if (ID != x)    // If object's ID variable is in fact not the same as what is given, then overload will return true
        return true;
    else
        return false;
}

bool UserInfo::operator == (int x)
{ // Overload allows private member ID to be compared to the integer that is given, if those are the same, overload returns true
    if (ID==x)
        return true;
    else
        return false;
}

void UserInfo::operator = (int x)
{ // Overload allows integer members of class to be set using given integer
  // My program, in login function, makes sure the user's ID cannot be 15,30,45, or 60, so that this overload knows whether it is being given an integer to set the ID or the time
    if (x==15 || x==30 || x==45 || x==60)
        time=x;
    else
        ID=x;
}

void UserInfo::operator = (string y)
{ // Overload allows string member of class, name, to be set using given string
    name=y;
}

void UserInfo::swap(UserInfo& x)
{
    UserInfo temp;
    temp=ID; temp=name; temp=time; temp.setIndex(stationIndex); temp.setLab(labIndex);
    ID=x.getID(); name=x.getName(); time=x.getTime(); stationIndex=x.getIndex(); labIndex=x.getLab();
    x=temp;
}

void LinkList::insertNode(UserInfo c)
{
    ListNode *newNode;
    ListNode *nodePtr;
    ListNode *previousNode=nullptr;
    newNode=new ListNode;
    newNode->object=c;
    if (!head)
    { // If the list is empty, we first set the head node
        head=newNode;
        newNode->next=nullptr; // If the list was empty, head was just filled and the pointer is a dangling pointer for now
    }
    else
    { // Otherwise, we traverse the list as long as we don't encounter dangling pointer, and while the index of the object of the new node is greater than current node's index
        nodePtr=head;
        previousNode=nullptr;
        while (nodePtr!=nullptr && c.getIndex()>nodePtr->object.getIndex())
        {
            previousNode=nodePtr;
            nodePtr=nodePtr->next;
        }
        if (previousNode==nullptr)
        { // If while loop never executes, the list head and newNode are reset
            head=newNode;
            newNode->next=nodePtr;
        }
        else
        { // If not, we rewrite previousNode and newNode based on nodePtr
            previousNode->next=newNode;
            newNode->next=nodePtr;
        }
    }
}

void LinkList::deleteNode(UserInfo c)
{
    ListNode *nodePtr;
    ListNode *previousNode;
    if(!head) // If head is not set, we must exit, as there is nothing to delete
        return;
    if(head->object.getID()==c.getID())
    { // If the head of the list is the specified ID, we delete it
        nodePtr=head->next;
        delete head;
        head=nodePtr;
    }
    else
    { // If not, we must traverse the list and look for it
        nodePtr=head;
        while(nodePtr!=nullptr && nodePtr->object.getID()!=c.getID())
        {
            previousNode=nodePtr;
            nodePtr=nodePtr->next;
        }
        if (nodePtr) // Once the while loop is completed, nodePtr now points to an address that holds the same UserInfo object, which is then deleted after rewiring list
        {
            previousNode->next=nodePtr->next;
            delete nodePtr;
        }
    }
}

//**************************************************
// Destructor                                      *
// This function deletes every node in the list.   *
// pre: n/a                                        *
// post: destroyed object                          *
//**************************************************

LinkList::~LinkList()
{
   ListNode *nodePtr;   // To traverse the list
   ListNode *nextNode;  // To point to the next node

   // Position nodePtr at the head of the list.
   nodePtr = head;

   // While nodePtr is not at the end of the list...
   while (nodePtr)
   {
       string ID=to_string(nodePtr->object.getID());
       int IDlength=ID.length();
       if (IDlength<MAXIDLENGTH)
       {
           std::cout << "User " << std::setw(MAXIDLENGTH-IDlength) << std::setfill('0') << '0' << nodePtr->object.getID() << ", "
           << nodePtr->object.getName() << ", is logging off. . ." << std::endl << std::endl;
       }
       else
           std::cout << "User " << nodePtr->object.getID() << ", " << nodePtr->object.getName() << ", is logging off . . ." << std::endl << std::endl;
      // Save a pointer to the next node.
      nextNode = nodePtr->next;

      // Delete the current node.
      delete nodePtr;
      // Position nodePtr at the next node.
      nodePtr = nextNode;

   }
   head=0; // List head no longer points to anything
}

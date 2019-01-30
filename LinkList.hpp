#ifndef LINKLIST_HPP_INCLUDED
#define LINKLIST_HPP_INCLUDED

#include "UserInfo.hpp"
#include <list>

using std::list;

class LinkList
{
public:
    LinkList()
    // Constructor
    {head=nullptr;}  // Initializes object with dangling pointer
    // Destructor
    ~LinkList();
    void insertNode(UserInfo);
    // Precondition: takes UserInfo object as input
    // Postcondition: adds node containing input object to a specified position in linked list
    void deleteNode(UserInfo);
    // Precondition: takes UserInfo object as input
    // Postcondition: deletes specified object from linked list
    friend void login(LinkList arr[],list<UserInfo>& waitList);
    // Precondition: takes array of linked lists
    // Postcondition: Simulates the login of user into a computer lab station
    friend void display(LinkList arr[]);
    //void display(UserInfo* arr[]);
    // Precondition: takes array of linked lists
    // Postcondition: Displays simulation of every computer station in a lab
    friend void logout(LinkList arr[],list<UserInfo>& waitList);
    // Precondition: takes array of linked lists
    // Postcondition: Simulates logout of user from a computer lab station
    friend void searchArr(LinkList arr[],list<UserInfo>& waitList);
    // Precondition: takes array of linked lists
    // Postcondition: Searches jagged array for user-specified ID to see if user is logged in to a station
    struct ListNode  // Creates struct "ListNode" variables of nodes containing a UserInfo object and a pointer
    {
        UserInfo object;
        struct ListNode *next;
    };
    ListNode *head; // ListNode pointer representing start of linked list
};


#endif // LINKLIST_HPP_INCLUDED

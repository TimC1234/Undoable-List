// Stringlist.h

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : Kuan Ting (Tim) Chou
// St.# : 301562019
// Email: tim_chou@sfu.ca
//
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.
//
/////////////////////////////////////////////////////////////////////////

//
// Do not use any other #includes or #pragmas in this file.
//

#pragma once

#include <cassert>
#include <iostream>
#include <string>

using namespace std;

class Stringlist
{
    
    struct Stack
    {
        string action; //stores the string command to undo
        Stack *next; //stores the next command to undo
        bool undone; //checks if an action has been already undone so it skips it
    };

    int cap;     // capacity
    string *arr; // array of strings
    int sz;      // size
    Stack* head = nullptr;
    bool removedAll = false; //checks if remove_all() has been called
    //
    // Helper function for throwing out_of_range exceptions.
    //
    void bounds_error(const string &s) const
    {
        throw out_of_range("Stringlist::" + s + " index out of bounds");
    }

    //
    // Helper function for checking index bounds.
    //
    void check_bounds(const string &s, int i) const
    {
        if (i < 0 || i >= sz)
        {
            bounds_error(s);
        }
    }

    //
    // Helper function for copying another array of strings.
    //
    void copy(const string *other)
    {
        for (int i = 0; i < sz; i++)
        {
            arr[i] = other[i];
        }
    }

    //
    // Helper function for checking capacity; doubles size of the underlying
    // array if necessary.
    //
    void check_capacity()
    {
        if (sz == cap)
        {
            cap *= 2;
            string *temp = new string[cap];
            for (int i = 0; i < sz; i++)
            {
                temp[i] = arr[i];
            }
            delete[] arr;
            arr = temp;
        }
    }

public:
    //
    // Default constructor: makes an empty StringList.
    //
    Stringlist()
        : cap(10), arr(new string[cap]), sz(0)
    {
    }

    //
    // Copy constructor: makes a copy of the given StringList.
    //
    // Does *not* copy the undo stack, or any undo information from other.
    //
    Stringlist(const Stringlist &other)
        : cap(other.cap), arr(new string[cap]), sz(other.sz)
    {
        copy(other.arr);
    }

    //
    // destructor
    //
    ~Stringlist()
    {
        //frees the rest of the undos if the user does not undo enough times

        while (head != nullptr)
        {
            Stack *temp = head;
            head = head->next;
            delete temp;
        }
        delete[] arr;
    }

    //
    // Assignment operator: makes a copy of the given StringList.
    //
    // undoable
    //
    // For undoing, when assigning different lists, the undo stack is NOT
    // copied:
    //
    //    lst1 = lst2; // lst1 undo stack is updated to be able to undo the
    //                 // assignment; lst1 does not copy lst2's stack
    //                 //
    //                 // lst2 is not change in any way
    //
    // Self-assignment is when you assign a list to itself:
    //
    //    lst1 = lst1;
    //
    // In this case, nothing happens to lst1. Both its string data and undo
    // stack are left as-is.
    //
    Stringlist &operator=(const Stringlist &other)
    {
        Stack* newAction = new Stack;
        if (empty())
        {
            //unique command if original list is empty
            //this will tell the undo function to remove all strings

            newAction->action = "Empty ";
            newAction->next = head;
            newAction->undone = false;
            head = newAction;
        }
        else
        {
            //combines all the strings into 1 big string
            //and stores it into an action for undo

            string command = toLongString();
            newAction->action = "set " + command;
            newAction->next = head;
            newAction->undone = false;
            head = newAction;
        }
        
        if (this != &other)
        {
            delete[] arr;
            cap = other.capacity();
            arr = new string[cap];
            sz = other.size();
            copy(other.arr);
        }
        return *this;
    }

    //
    // Returns the number of strings in the list.
    //
    int size() const { return sz; }

    //
    // Returns true if the list is empty, false otherwise.
    //
    bool empty() const { return size() == 0; }

    //
    // Returns the capacity of the list, i.e. the size of the underlying array.
    //
    int capacity() const { return cap; }

    //
    // Returns the string at the given index.
    //
    string get(int index) const
    {
        check_bounds("get", index);
        return arr[index];
    }

    //
    // Returns the index of the first occurrence of s in the list, or -1 if s is
    // not in the lst.
    //
    int index_of(const string &s) const
    {
        for (int i = 0; i < sz; i++)
        {
            if (arr[i] == s)
            {
                return i;
            }
        }
        return -1;
    }

    //
    // Returns true if s is in the list, false otherwise.
    //
    bool contains(const string &s) const
    {
        return index_of(s) != -1;
    }

    //
    // Returns a string representation of the list.
    //
    string to_string() const
    {
        string result = "{";
        for (int i = 0; i < size(); i++)
        {
            if (i > 0)
            {
                result += ", ";
            }
            result += "\"" + get(i) + "\"";
        }
        return result + "}";
    }

    //helper function similar to one above
    //combines all the strings into 1 big string separated by commas
    string toLongString() const
    {
        string result = "";
        for (int i = 0; i < size(); i++)
        {
            if (i > 0)
            {
                result += ", ";
            }
            result += get(i);
        }
        return result;
    }

    //
    // Sets the string at the given index.
    //
    // undoable
    //
    void set(int index, string value)
    {
        
        check_bounds("set", index);
        
        Stack* newAction = new Stack; //creates a new command to store in the undo stack
        string indexString = std::to_string(index); //turns the index from int to a string
        newAction->action = "SET " + indexString + " " + arr[index]; //makes the command
        newAction->next = head;
        newAction->undone = false;
        head = newAction;

        arr[index] = value;
        
    }

    //
    // Insert s before index; if necessary, the capacity of the underlying array
    // is doubled.
    //
    // undoable
    //
    void insert_before(int index, const string &s)
    {
       
        if (index < 0 || index > sz)
        { // allows insert at end, i == sz
            bounds_error("insert_before");
        }
        check_capacity();

        Stack* newAction = new Stack;
        string indexString = std::to_string(index);
        newAction->action = "REMOVE " + indexString;
        newAction->next = head;
        newAction->undone = false;
        head = newAction;

        for (int i = sz; i > index; i--)
        {
            arr[i] = arr[i - 1];
        }
        arr[index] = s;
        sz++;
    }

    //
    // Appends s to the end of the list; if necessary, the capacity of the
    // underlying array is doubled.
    //
    // undoable
    //
    void insert_back(const string &s)
    {
        insert_before(size(), s);
    }

    //
    // Inserts s at the front of the list; if necessary, the capacity of the
    // underlying array is doubled.
    //
    // undoable
    //
    void insert_front(const string &s)
    {
        insert_before(0, s);
    }

    //
    // Removes the string at the given index; doesn't change the capacity.
    //
    // undoable
    //
    void remove_at(int index)
    {
        check_bounds("remove_at", index);

        Stack* newAction = new Stack;
        string indexString = std::to_string(index);
        newAction->action = "INSERT " + indexString + " " + arr[index];
        newAction->next = head;
        newAction->undone = false;
        head = newAction;

        for (int i = index; i < sz - 1; i++)
        {
            arr[i] = arr[i + 1];
        }
        sz--;
    }

    void remove_allHelper(int index)
    {
        if (removedAll == true)
        {
            Stack* newAction = new Stack;
            string command = toLongString(); //combines all the strings into 1 big string
            newAction->action = "Set " + command;
            newAction->next = head;
            newAction->undone = false;
            head = newAction;
        }

        //stops the function from running the for loop multiple times
        removedAll = false;
        
        //removes the words code taken from remove_at method
        for (int i = index; i < sz - 1; i++)
        {
            arr[i] = arr[i + 1];
        }
        sz--;
    }

    //
    // Removes all strings from the list; doesn't change the capacity.
    //
    // undoable
    //
    void remove_all()
    {
        removedAll = true;
        while (sz > 0)
        {
            //uses helper function to remove all the words
            //helper function adds the command to undo
            remove_allHelper(sz - 1);
        }
    }

    //
    // Removes the first occurrence of s in the list, and returns true. If s is
    // nowhere in the list, nothing is removed and false is returned.
    //
    // undoable
    //
    bool remove_first(const string &s)
    {
        int index = index_of(s);
        if (index == -1)
        {
            return false;
        }
        remove_at(index);
        return true;
    }

    
   //helper function for removeAll
   //how to use str.find taken from https://cplusplus.com/reference/string/string/find/
   void splitAndInsert(const string &input) 
   {
        //tracks starting position and position of commas
        int start = 0;
        int commaPos;

        //assigns the index position of a comma to commaPos
        //checks if there are commas in the string using != std::string::npos
        while ((commaPos = input.find(',', start)) != std::string::npos) 
        {
            //creates a new string that only has the next word omitting the commas
            //and other subsequent words
            string command = input.substr(start, commaPos - start);
            insert_back(command);
            //commaPos + 2 because it skips the comma and a blank space
            start = commaPos + 2;
        }
        // Adds the last word to the list as the last word won't have a comma
        string lastCommand = input.substr(start);
        insert_back(lastCommand);
    }

    //
    // Undoes the last operation that modified the list. Returns true if a
    // change was undone.
    //
    // If there is nothing to undo, does nothing and returns false.
    //

    bool undo()
    {
        //checks if undo stack is empty
        if (head == nullptr)
        {
            return false;
        }
        //creates a new stack variable and have it skip all undone actions
        Stack* current = head;
        while (current != nullptr && current->undone)
        {
            current = current->next;
        }
        if (current == nullptr)
        {
            return false;
        }

        //this assigns the first 3 characters of current->action to command
        //this will allow me to check which action it corresponds to
        //how to use str.substr(a, b) is from https://cplusplus.com/reference/string/string/substr/
        
        string command = current->action.substr(0, 3);
        
        //for insert_before()
        if (command == "REM")
        {
            string command2 = current->action.substr(7);
            
            //converts the string command2 to an int
            //taken from https://cplusplus.com/reference/string/stoi/
            //it will automatically stop when it detects a non int character
            //toRemove is the index of the character that should be removed
            
            int toRemove = stoi(command2);
            remove_at(toRemove);
    
        }
        
        //for remove_at()
        else if (command == "INS")
        {
            string command2 = current->action.substr(7);
            int index = stoi(command2);
            int digits = 0;
            int temp = index;
            
            //this while loop allows me to know how many characters long the index is
            //so I can parse through that section and skip to the string portions
            
            while (temp != 0)
            {
                digits++;
                temp /= 10;
            }
            
            //extra case because 0 is 1 character long 
            
            if (index == 0)
            {
                digits++;
            }
           
            //the 1 from 1 + digits accounts for the blank space in between
            //for example "INSERT 0 bob" the + 1 skips the space
            
            string input = command2.substr(1 + digits);
            insert_before(index, input);
        }
        
        //for remove_all()
        else if (command == "Set")
        {
            string command2 = current->action.substr(4);
            
            //uses helper method to parse through the big string and separate each string
            //by looking at the commas. The individual strings will be inserted back
            //in the corresponding order
            
            splitAndInsert(command2);
        }
        
        //for operator=
        else if (command == "set")
        {
            //taken from original remove_all()
            while (sz > 0)
            {
                remove_allHelper(sz - 1);
            }
            //clears the list then adds back the original strings
            string command2 = current->action.substr(4);
            //uses helper function to add all the individual strings back to the list
            splitAndInsert(command2);
        }
        
        //edge case for operator= if original list was empty
        else if (command == "Emp")
        {
            //removes all the strings in the list 
            //reverts it back to empty list
            while (sz > 0)
            {
                remove_allHelper(sz - 1);
            }
        }
        
        //for set()
        else
        {
            //creates another string that skips the first 4 characters of current->action
            string command2 = current->action.substr(4);
            //similar to the if statement for "INS"
            int index = stoi(command2);
            int temp = index;
            int digits = 0;
            //determines number of characters the index is and allows me to skip it
            while (temp != 0)
            {
                digits++;
                temp /= 10;
            }
            //extra case because 0 is 1 character long
            if (index == 0)
            {
                digits++;
            }

            string input = command2.substr(digits + 1);
            set(index, input);
        }

        //marks the action as done so the undo function knows to skip it
        current->undone = true;

        //updates the head node
        //deletes previous undone actions
        
        while (head != current)
        {
            Stack *temp = head;
            head = head->next;
            delete temp;
        }
        //extra case as the while loop stops 1 short so will cause memory leak otherwise
        Stack *temp = head;
        head = head->next;
        delete temp;
        return true;
    }

}; // class Stringlist

//
// Prints list to in the format {"a", "b", "c"}.
//
ostream &operator<<(ostream &os, const Stringlist &lst)
{
    return os << lst.to_string();
}

//
// Returns true if the two lists are equal, false otherwise.
//
// Does *not* consider any undo information when comparing two Stringlists. All
// that matters is that they have the same strings in the same order.
//
bool operator==(const Stringlist &a, const Stringlist &b)
{
    if (a.size() != b.size())
    {
        return false;
    }
    for (int i = 0; i < a.size(); i++)
    {
        if (a.get(i) != b.get(i))
        {
            return false;
        }
    }
    return true;
}

//
// Returns true if the two lists are not equal, false otherwise.
//
// Does *not* consider any undo information when comparing two Stringlists.
//
bool operator!=(const Stringlist &a, const Stringlist &b)
{
    return !(a == b);
}
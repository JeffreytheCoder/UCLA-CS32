#include <iostream>
#include <cassert>
using namespace std;

class StringList
{
public:
    StringList();
    ~StringList();
    void addToFront(char v);
    void addToRear(char v);
    // You may declare other member functions for your test code to call
    void eraseEvens();
    void dump();
private:
    struct Node
    {
        char  value;
        Node* next;
    };
    Node* head;  // points to first Node in the list
};
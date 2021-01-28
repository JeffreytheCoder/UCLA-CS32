#include <iostream>
using namespace std;

class LinkedList {
public:
    LinkedList();
    ~LinkedList();
    void addToList(int value); // add to the end of the linked list
    void reverse(); // Reverse the linked list
    bool findNthFromLast(int N, int& value);
    void output();

private:
    struct Node
    {
        int num;
        Node* next;
    };
    Node* head;
};
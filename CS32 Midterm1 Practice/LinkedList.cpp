#include "LinkedList.h"

LinkedList::LinkedList() {
    head = nullptr;
}

LinkedList::~LinkedList() {
    Node* p = head;
    while (p != nullptr) {
        Node* n = p->next;
        delete p;
        p = n;
    }
}

void LinkedList::addToList(int value) {
    if (head == nullptr) {
        Node* n = new Node;
        n->num = value;
        n->next = nullptr;
        head = n;
    }
    else {
        Node* p = head;
        while (p->next != nullptr) {
            p = p->next;
        }
        Node* n = new Node;
        n->num = value;
        n->next = nullptr;
        p->next = n;
    }
}

void LinkedList::reverse()
{
    Node* nextNode = nullptr, * prevNode = nullptr, * current = head;
    while (current) {
        // Hint: Only 4 lines of codes are needed inside the while loop
        nextNode = current->next;
        current->next = prevNode;
        prevNode = current;
        current = nextNode;
    }
    head = prevNode;
}

//bool LinkedList::findNthFromLast(int N, int& value) {
//    reverse();
//    Node* p = head;
//    int idx = 1;
//    while (p != nullptr) {
//        if (idx == N) {
//            value = p->num;
//            reverse();
//            return true;
//        }
//        p = p->next;
//        idx++;
//    }
//    reverse();
//    return false;
//}

bool LinkedList::findNthFromLast(int N, int& value) {
    Node* p = head;
    int size = 0;
    while (p != nullptr) {
        size++;
        p = p->next;
    }
    int pos = size - N;
    if (pos < 0)
        return false;
    int idx = 0;
    p = head;
    while (p != nullptr) {
        if (idx == pos) {
            value = p->num;
            return true;
        }
        p = p->next;
        idx++;
    }
}

void LinkedList::output() {
    Node* ptr = head;
    cout << "The elements in the list are: ";
    while (ptr != nullptr) {
        cout << ptr->num << " ";
        ptr = ptr->next;
    }
    cout << endl;
}

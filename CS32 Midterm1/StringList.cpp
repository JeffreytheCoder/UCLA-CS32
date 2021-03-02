#include "StringList.h"

StringList::StringList()
{
	head = nullptr;
}

StringList::~StringList() {
	Node* p = head;
	while (p != nullptr) {
		Node* n = p->next;
		delete p;
		p = n;
	}
}

void StringList::addToFront(char v) { // point new node to head and makes it head
	Node* p = new Node;
	p->value = v;
	p->next = head;
	head = p;
}

void StringList::addToRear(char v) { // loop linked list to last node and set its next to the new node
	if (head == nullptr)
		addToFront(v);
	else {
		Node* p = new Node;
		p = head;
		while (p->next != nullptr) {
			p = p->next;
		}
		Node* n = new Node;
		n->value = v;
		n->next = nullptr;
		p->next = n;
	}
}

void StringList::eraseEvens() {
	Node* p = head;
	while (p != nullptr && p->next != nullptr) {
		Node* d = p->next;
		p->next = d->next;
		delete d;
		p = p->next;
	}
}

void StringList::dump() {
	Node* p = head;
	while (p != nullptr) {
		cout << p->value << endl;
		p = p->next;
	}
}
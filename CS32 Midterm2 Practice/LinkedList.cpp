#include <iostream>

using namespace std;

class LinkedList {
public:
	LinkedList() {
		head = nullptr;
	}
	~LinkedList() {
		removeNodes(head);
	}
	void printReverse() const {
		printReverseHelper(head);
		cout << endl;
	};
	void reverse() {
		head = reverseHelper(head, nullptr);
	};
	int sum() {
		return sumHelper(head);
	}
private:
	struct Node {
		int num;
		Node* next;
	};
	Node* head;

	void printReverseHelper(const Node* p) const {
		if (p == nullptr)
			return;
		printReverseHelper(p->next);
		cout << p->num << "";
	}
	Node* reverseHelper(Node* current, Node* previous) {
		if (current == nullptr)
			return previous;  // return last node
		Node* last_node = reverseHelper(current->next, current);  // locate last node as new header
		current->next = previous;  // link back to previous
		return last_node;
	}
	int sumHelper(const Node* p) const {
		if (p->next == nullptr)
			return p->num;
		return p->num + sumHelper(p->next);
	}
	void removeNodes(Node* p) {
		if (p == nullptr)
			return;
		removeNodes(p->next);
		delete p;
	}
};
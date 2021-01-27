#include "Sequence.h"

Sequence::Sequence() {
	m_size = 0;
	head = nullptr;
};

Sequence::Sequence(const Sequence& src) {
	head = nullptr;
	m_size = src.m_size;
	Node* node = nullptr;
	Node* srcNode = src.head;

	// copy every nodes in src to this sequence
	while (srcNode != nullptr) {
		Node* p = new Node;
		p->value = srcNode->value;
		p->next = nullptr;

		if (node == nullptr)
			head = p;
		else {
			node->next = p;
			p->prev = node;
		}
		
		node = p;
		srcNode = srcNode->next;
	}
}

Sequence::~Sequence() {
	Node* p = head;
	Node* n;
	// delete every nodes in the list
	while (p != nullptr) {
		n = p->next;
		delete p;
		p = n;
	}
}

Sequence& Sequence::operator=(const Sequence& src) {
	// return this if this and src are the same
	if (&src == this)
		return *this;

	// delete every nodes of this list
	Node* p = head;
	Node* n;
	while (p != nullptr) {
		n = p->next;
		delete p;
		p = n;
	}

	// copy every nodes from src to this list
	head = nullptr;
	m_size = src.m_size;
	Node* node = nullptr;
	Node* srcNode = src.head;

	while (srcNode != nullptr) {
		Node* p = new Node;
		p->value = srcNode->value;
		p->next = nullptr;

		if (node == nullptr)
			head = p;
		else {
			node->next = p;
			p->prev = node;
		}

		node = p;
		srcNode = srcNode->next;
	}
	return *this;
}

bool Sequence::empty() const {
	return size() == 0;
};

int Sequence::size() const {
	return m_size;
};

int Sequence::insert(int pos, const ItemType& value) {
	if (pos < 0 || pos > size())
		return -1;

	// insert as header
	if (pos == 0) {
		Node* n = new Node;
		n->value = value;
		n->next = head;
		n->prev = nullptr;
		if (head != nullptr) // if a header exists
			head->prev = n;
		head = n;
		m_size++;
		return 0;
	}

	// insert as non-header
	int idx = 0;
	Node* p = head;
	while (p != nullptr) {	
		idx++;	// keep idx one index after p so p is always the node above n inserted
		if (pos == idx) {
			Node* n = new Node;
			n->value = value;
			n->next = p->next;
			n->prev = p;
			if (p->next != nullptr)	// if n is not the last node
				p->next->prev = n;
			p->next = n;
			m_size++;
			return pos;
		}
		p = p->next;
	}
	return -1;
}

int Sequence::insert(const ItemType& value) {
	// insert as head if no node exists or value is smaller than header
	if (size() == 0 || value < head->value)
		return insert(0, value);

	// insert as non-header except last node
	int pos = 1;
	Node* p = head;
	while (p->next != nullptr) {	// loop until before the last node since need to compare n with next node
		if (value <= p->next->value) {	// insert if the next node is larger than n
			Node* n = new Node;
			n->value = value;
			n->next = p->next;
			n->prev = p;
			p->next->prev = n;
			p->next = n;
			m_size++;
			return pos;
		}
		p = p->next;
		pos++;
	}

	// insert as last node
	Node* n = new Node;
	n->value = value;
	n->next = p->next;
	n->prev = p;
	p->next = n;
	m_size++;
	return pos;
};

bool Sequence::erase(int pos) {
	if (pos < 0 || pos >= size())
		return false;

	// erase header
	if (pos == 0) {
		if (head->next != nullptr) {	// if header has nodes behind
			head = head->next;
			delete head->prev;
			head->prev = nullptr;
		}
		else {	// if the only node is header
			delete head;
			head = nullptr;
		}
		m_size--;
		return true;
	}

	// erase non-header except last node
	int idx = 1;
	Node* p = head->next;
	while (p->next != nullptr) {
		if (pos == idx) {
			p->prev->next = p->next;
			p->next->prev = p->prev;
			delete p;
			m_size--;
			return true;
		}
		p = p->next;
		idx++;
	}

	// erase last node
	if (pos == idx) {
		if (p == head)
			head = nullptr;
		else
			p->prev->next = nullptr;
		delete p;
		m_size--;
		return true;
	}
	return false;
};

int Sequence::remove(const ItemType& value) {
	if (empty())
		return 0;

	// remove header
	if (size() == 1) {
		if (head->value != value)
			return 0;
		delete head;
		head = nullptr;
		m_size--;
		return 1;
	}

	// remove non-header except last node
	int count = 0;
	Node* p = head;
	while (p->next != nullptr) {
		if (p->value == value) {
			if (p == head) {	// if after former removal, the current node becomes header
				head = head->next;
				delete head->prev;
				head->prev = nullptr;
				p = head;
				m_size--;
				count++;
				continue;
			}
			else {
				p->prev->next = p->next;
				p->next->prev = p->prev;
				Node* n = p->prev;
				delete p;
				p = n;
				m_size--;
				count++;
			}
		}
		p = p->next;
	}

	// remove last node
	if (p->value == value) {
		if (p == head)
			head = nullptr;
		else
			p->prev->next = nullptr;
		delete p;
		m_size--;
		count++;
	}
	return count;
};

bool Sequence::get(int pos, ItemType& value) const {
	if (pos < 0 || pos >= size())
		return false;

	// loop until pos, get value and return true
	int idx = 0;
	Node* p = head;
	while (p != nullptr) {
		if (idx == pos) {
			value = p->value;
			return true;
		}
		p = p->next;
		idx++;
	}
	return false;
};

bool Sequence::set(int pos, const ItemType& value) {
	Node* p = head;
	int idx = 0;

	// loop until pos, set value and return true
	while (p != nullptr) {
		if (idx == pos) {
			p->value = value;
			return true;
		}
		p = p->next;
		idx++;
	}
	return false;
};

int Sequence::find(const ItemType& value) const {
	// loop until find value and return index
	Node* p = head;
	int idx = 0;
	while (p != nullptr) {
		if (p->value == value) {
			return idx;
		}
		p = p->next;
		idx++;
	}
	return -1;
};

void Sequence::swap(Sequence& other) {
	// swap size
	int sizeTemp = m_size;
	m_size = other.m_size;
	other.m_size = sizeTemp;

	// swap header (and the whole list linked behind)
	Node* headTemp = head;
	head = other.head;
	other.head = headTemp;
};

int subsequence(const Sequence& seq1, const Sequence& seq2) {
	ItemType x, y;
	for (int i = 0; i < seq1.size(); i++) {
		// for each node in seq1, check if it and the following nodes (with seq2-size) are same with seq2; if so, return index
		for (int j = 0; j < seq2.size(); j++) {
			seq1.get(i + j, x);
			seq2.get(j, y);
			if (x != y) {
				break;
			}
			if (j == seq2.size() - 1)
				return i;
		}
	}
	return -1;
};

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result) {
	Sequence temp;	// set a temp for result in case result contains value OR seq1 or seq2 is result

	// get the minimum size of seq1 & seq2, set the larger one as seqLarger
	int min = std::min(seq1.size(), seq2.size());
	Sequence seqLarger;
	if (min == seq1.size())
		seqLarger = seq2;
	else
		seqLarger = seq1;

	ItemType x, y, z;
	if (min == 0) {	// if one of seq1 & seq2 is empty
		for (int j = 0; j < seqLarger.size(); j++) {	// insert nodes of the nonempty seq
			seqLarger.get(j, z);
			temp.insert(j, z);
		}
	}
	else {
		int i;
		for (i = 0; i < min; i++) {	// insert each node in seq1 & seq2 until min size
			seq1.get(i, x);
			seq2.get(i, y);
			temp.insert(2 * i, x);
			temp.insert(2 * i + 1, y);
		}

		int k = min * 2;
		for (int j = i; j < seqLarger.size(); j++) {	// after min x 2, insert the left nodes in  seqLarger
			seqLarger.get(j, z);
			temp.insert(k, z);
			k++;
		}
	}
	result = temp;
}
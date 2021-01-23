#include "newSequence.h"

Sequence::Sequence(int size) {
	m_size = 0;
	m_maxSize = size;
	m_items = new ItemType[m_maxSize];
}

Sequence::Sequence() {
	m_size = 0;
	m_maxSize = m_maxSize;
	m_items = new ItemType[m_maxSize];
};

Sequence::Sequence(const Sequence& src) {
	m_size = src.size();
	m_maxSize = src.m_maxSize;
	m_items = new ItemType[m_maxSize];
	for (int i = 0; i < size(); i++) {
		m_items[i] = src.m_items[i];
	}
}

Sequence& Sequence::operator=(const Sequence& src) {
	if (&src == this)
		return *this;
	delete[] m_items;
	m_size = src.size();
	m_maxSize = src.m_maxSize;
	m_items = new ItemType[m_maxSize];
	for (int i = 0; i < size(); i++) {
		m_items[i] = src.m_items[i];
	}
	return *this;
}

Sequence::~Sequence() {
	delete[] m_items;
}

bool Sequence::empty() const {
	if (m_size == 0)
		return true;
	return false;
};

int Sequence::size() const {
	return m_size;
};

int Sequence::insert(int pos, const ItemType& value) {
	if (pos < 0 || pos > size() || size() >= m_maxSize)
		return -1;
	m_size++;
	for (int i = size() - 1; i > pos; i--) {
		m_items[i] = m_items[i - 1];
	}
	m_items[pos] = value;
	return pos;
};

int Sequence::insert(const ItemType& value) {
	if (m_size >= m_maxSize)
		return -1;
	int pos = 0;
	while (pos < size() && value > m_items[pos]) {
		pos++;
	}
	return insert(pos, value);
};

bool Sequence::erase(int pos) {
	if (pos < 0 || pos >= size())
		return false;
	m_size--;
	for (int i = pos; i < size(); i++) {
		m_items[i] = m_items[i + 1];
	}
	return true;
};

int Sequence::remove(const ItemType& value) {
	int removeCount = 0;
	// since change of m_size don't change returned value of size(), use m_size here
	for (int i = 0; i < m_size; i++) {
		if (m_items[i] == value) {
			erase(i);
			removeCount++;
			i--;
		}
	}
	return removeCount;
};

bool Sequence::get(int pos, ItemType& value) const {
	if (pos < 0 || pos >= size())
		return false;
	value = m_items[pos];
	return true;
};

bool Sequence::set(int pos, const ItemType& value) {
	if (pos < 0 || pos >= size())
		return false;
	m_items[pos] = value;
	return true;
};

int Sequence::find(const ItemType& value) const {
	for (int i = 0; i < size(); i++) {
		if (m_items[i] == value)
			return i;
	}
	return -1;
};

void Sequence::swap(Sequence& other) {
	ItemType* itemTemp = other.m_items;
	other.m_items = m_items;
	m_items = itemTemp;

	int sizeTemp = other.m_size;
	other.m_size = m_size;
	m_size = sizeTemp;

	int maxSizeTemp = other.m_maxSize;
	other.m_maxSize = m_maxSize;
	m_maxSize = maxSizeTemp;
}
#include "ScoreList.h"

ScoreList::ScoreList() {
	m_sequence = Sequence();
};

bool ScoreList::add(ItemType score) {
	if (size() >= DEFAULT_MAX_ITEMS || score < 0 || score > 100)
		return false;
	m_sequence.insert(score);
	return true;
};

bool ScoreList::remove(ItemType score) {
	int pos = m_sequence.find(score);
	if (pos == -1)
		return false;
	return m_sequence.erase(pos);
};

int ScoreList::size() const {
	return m_sequence.size();
};

ItemType ScoreList::minimum() const {
	if (size() == 0)
		return NO_SCORE;
	ItemType min, cur;
	m_sequence.get(0, min);
	for (int i = 0; i < size(); i++) {
		m_sequence.get(i, cur);
		if (cur < min)
			min = cur;
	}
	return min;
}

ItemType ScoreList::maximum() const {
	if (size() == 0)
		return NO_SCORE;
	ItemType max, cur;
	m_sequence.get(0, max);
	for (int i = 0; i < size(); i++) {
		m_sequence.get(i, cur);
		if (cur > max)
			max = cur;
	}
	return max;
}
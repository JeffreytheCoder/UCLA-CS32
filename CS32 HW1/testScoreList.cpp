#include "ScoreList.h"
#include <cassert>
#include <iostream>

using namespace std;

int main() {
	ScoreList s;
	s.add(97);
	s.add(98);
	s.add(99);
	s.add(100);
	s.remove(98);
	assert(s.size() == 3);
	assert(s.maximum() == 100);
	assert(s.minimum() == 97);
	cout << "all test passed" << endl;
	return 1;
}
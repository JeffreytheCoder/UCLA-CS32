#include <iostream>
#include <vector>
using namespace std;

bool isQueue(const vector<int>& seq1, vector<int>& seq2) {
	for (int i = 0; i < seq1.size(); i++) {
		if (seq1[i] != seq2[i])
			return false;
	}
	return true;
}

bool isStack(const vector<int>& seq1, const vector<int>& seq2) {
	for (int i = 0; i < seq1.size(); i++) {
		if (seq1[i] != seq2[seq2.size() - i - 1])
			return false;
	}
	return true;
}
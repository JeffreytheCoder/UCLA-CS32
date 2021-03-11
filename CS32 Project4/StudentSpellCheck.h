#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

const int ALPHABET_SIZE = 27;  // 26 letters and '

class StudentSpellCheck : public SpellCheck {
public:
	StudentSpellCheck();
	virtual ~StudentSpellCheck();
	int charToIndex(char c);
	void insertNode(string word);
	bool searchNode(string word);
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
	struct TrieNode {
		TrieNode() {
			for (int i = 0; i < ALPHABET_SIZE; i++) {
				children[i] = nullptr;
			}
			isEndOfWord = false;
		}
		TrieNode* children[ALPHABET_SIZE];
		bool isEndOfWord;
	};
	void clearTrie(TrieNode* n) {
		if (n->children == nullptr) {
			return;
		}
		for (int i = 0; i < ALPHABET_SIZE; i++) {
			if (!n->children[i]) {
				clearTrie(n->children[i]);
			}
		}
		delete n;
	}
	TrieNode* m_root;
};

#endif // STUDENTSPELLCHECK_H_
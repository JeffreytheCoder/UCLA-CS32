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
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
	int charToIndex(char c) {
		if (c == 39) {  // is /
			return 26;
		}
		else {
			return c - 'a';
		}
	};

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
	TrieNode* m_root;

	void insertNode(string word) {
		struct TrieNode* n = m_root;
		for (int i = 0; i < word.length(); i++) {
			int index = charToIndex(word.at(i));
			if (n->children[index] == nullptr)
				n->children[index] = new TrieNode;
			n = n->children[index];
		}
		n->isEndOfWord = true;
	};

	bool searchNode(string word) {
		if (word == "") return true;
		struct TrieNode* n = m_root;
		for (int i = 0; i < word.length(); i++) {
			int index = charToIndex(word.at(i));
			if (n->children[index] == nullptr) {
				return false;
			}
			n = n->children[index];
		}
		return n->isEndOfWord;
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
};

#endif // STUDENTSPELLCHECK_H_
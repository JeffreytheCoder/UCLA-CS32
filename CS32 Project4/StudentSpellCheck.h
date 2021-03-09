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
    StudentSpellCheck() { }
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
	struct TrieNode {
		TrieNode* children[ALPHABET_SIZE];
		bool isEndOfWord;
	};
	TrieNode* m_root;
	TrieNode* createNode() {
		struct TrieNode* n = new TrieNode;
		n->isEndOfWord = false;
		for (int i = 0; i < ALPHABET_SIZE; i++) {
			n->children[i] = nullptr;
		}
		return n;
	};
	void insertNode(string word) {
		struct TrieNode* n = m_root;
		for (int i = 0; i < word.length(); i++) {
			int index = word[i] - 'a';
			if (!n->children[index])
				n->children[index] = createNode();
			n = n->children[index];
		}
		n->isEndOfWord = true;
	}
	bool searchNode(string word) {
		struct TrieNode* n = m_root;
		for (int i = 0; i < word.length(); i++) {
			int index = word[i] - 'a';
			if (!n->children[index])
				return false;
			n = n->children[index];
		}
		return (n != nullptr && n->isEndOfWord);
	}
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

#endif  // STUDENTSPELLCHECK_H_

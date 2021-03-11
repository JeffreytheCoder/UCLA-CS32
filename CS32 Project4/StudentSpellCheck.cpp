#include "StudentSpellCheck.h"
#include <string>
#include <vector>

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::StudentSpellCheck() {
	m_root = new TrieNode;
}

StudentSpellCheck::~StudentSpellCheck() {
	clearTrie(m_root);
}

int StudentSpellCheck::charToIndex(char c) {
	if (c == 39) {  // is /
		return 26;
	}
	else {
		return c - 'a';
	}
}

void StudentSpellCheck::insertNode(string word) {
	struct TrieNode* n = m_root;
	for (int i = 0; i < word.length(); i++) {
		int index = charToIndex(word.at(i));
		if (n->children[index] == nullptr)
			n->children[index] = new TrieNode;
		n = n->children[index];
	}
	n->isEndOfWord = true;
}
bool StudentSpellCheck::searchNode(string word) {
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
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
	ifstream inputFile(dictionaryFile);
	if (inputFile.is_open()) {
		string word = "";
		while (getline(inputFile, word)) {
			insertNode(word);
		}
		inputFile.close();
		return true;
	}
	else {
		cout << "Unable to open dictionary file";
		return false;
	}
	return false;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	string loweredWord = "";
	for (int i = 0; i < word.length(); i++) {
		loweredWord.push_back(tolower(word.at(i)));
	}

	if (searchNode(loweredWord)) {
		return true;
	}
	suggestions.clear();

	for (int i = 0; i < loweredWord.length(); i++) {
		string strBefore = loweredWord.substr(0, i);
		string strAfter = loweredWord.substr(i + 1);
		for (int j = 0; j < ALPHABET_SIZE; j++) {
			char alter;
			if (j == ALPHABET_SIZE - 1) {
				alter = '\'';
			}
			else {
				alter = 'a' + j;
			}
			string suggestion = strBefore + alter + strAfter;
			if (searchNode(suggestion)) {
				suggestions.push_back(suggestion);
			}
			if (suggestions.size() >= max_suggestions) {
				break;
			}
		}
	}
	return false;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	string loweredLine = "";
	for (char c : line) {
		loweredLine += putchar(tolower(c));
	}
	loweredLine += ' ';  // To detect the last word in line
	
	string word = "";
	problems.clear();
	for (int i = 0; i < loweredLine.length(); i ++) {
		char c = loweredLine[i];
		if (isalpha(c) || c == '\'') {
			word += c;
		}
		else {
			if (!searchNode(word)) {
				Position p = { i - word.length() , i - 1 };
				problems.push_back(p);
			}
			word = "";
		}
	}
}
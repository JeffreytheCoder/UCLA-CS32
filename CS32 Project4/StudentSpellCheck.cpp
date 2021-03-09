#include "StudentSpellCheck.h"
#include <string>
#include <vector>

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::StudentSpellCheck() {
	m_root = createNode();
}

StudentSpellCheck::~StudentSpellCheck() {
	clearTrie(m_root);
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
	ifstream inputFile(dictionaryFile);
	if (inputFile.is_open()) {
		string word = "";
		/*char c;
		while (inputFile.get(c)) {
			if (c == '\n') {
				insertNode(word);
				word = "";
				continue;
			}
			word += c;
		}*/
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
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	for (char c : word) {
		putchar(tolower(c));
	}
	cout << word << endl;

	if (searchNode(word)) {
		return true;
	}
	suggestions.clear();

	//TrieNode* n = m_root;
	/*bool isDiffer = false;*/

	for (int i = 0; i < word.length(); i++) {
		string strBefore = word.substr(0, i);
		string strAfter = word.substr(i + 1);
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
		/*
		int index = word[i] - 'a';
		if (!n->children[index])
			if (!isDiffer) {
				isDiffer = true;
				for (int j = 0; j < ALPHABET_SIZE; j++) {
					if (n->children[j]) {
						char alter;
						if (j == ALPHABET_SIZE - 1) {
							alter = '\'';
						}
						else {
							alter = 'a' + j;
						}
						string suggestion = word.substr(0, i) + alter + word.substr(i + 1);
						if (searchNode(suggestion)) {
							suggestions.push_back(suggestion);
						}
						if (suggestions.size() >= max_suggestions) {
							break;
						}
					}
				}
			}
			else {
				suggestions.clear();
				return false;
			}
		n = n->children[index];*/
	}
	return false;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	string loweredLine = "";
	for (char c : line) {
		loweredLine += putchar(tolower(c));
	}
	line.push_back(' ');  // 
	
	string word;
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
		}
	}
}

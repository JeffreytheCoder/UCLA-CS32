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
	for (int i = 0; i < line.length(); i++) {
		loweredLine.push_back(tolower(line.at(i)));
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
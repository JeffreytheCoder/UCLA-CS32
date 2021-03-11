#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo) {
	m_lines.push_back("");
	m_it = m_lines.begin();
	m_row = 0;
	m_col = 0;
}

StudentTextEditor::~StudentTextEditor() {
	m_lines.clear();
}

bool StudentTextEditor::load(std::string file) {
	ifstream inputFile(file);
	if (inputFile.is_open()) {
		reset();
		char c;
		string line;
		while (inputFile.get(c)) {
			if (c == '\r')
				continue;
			if (c == '\n') {
				m_lines.push_back(line);
				line = "";
				continue;
			}
			line += c;
		}
		m_it = m_lines.begin();
		inputFile.close();
		return true;
	}
	else {
		cout << "Unable to open input file";
		return false;
	}
}

bool StudentTextEditor::save(std::string file) {
	ofstream outputFile(file);
	if (outputFile.is_open()) {
		for (string line : m_lines) {
			outputFile << line << '\n';
		}
		outputFile.close();
	}
	else {
		cout << "Unable to open output file";
		return false;
	}
}

void StudentTextEditor::reset() {
	// TODO: reset Undo
	m_lines.clear();
	m_it = m_lines.begin();
	m_row = 0;
	m_col = 0;
}

void StudentTextEditor::move(Dir dir) {
	if (dir == UP) {
		if (m_row != 0) {
			m_row--;
			m_it--;
			if (m_col > m_it->length()) {
				m_col = m_it->length();
			}
		}
	}
	else if (dir == DOWN) {
		if (m_row != m_lines.size() - 1) {
			m_row++;
			m_it++;
			if (m_col > m_it->length()) {
				m_col = m_it->length();
			}
		}
	}
	else if (dir == LEFT) {
		if (m_col == 0 && m_row != 0) {
			m_row--;
			m_it--;
			m_col = m_it->length() - 1;
		}
		else {
			m_col--;
		}
	}
	else if (dir == RIGHT) {
		if (m_col == m_it->length() - 1 && m_row < m_lines.size() - 1) {
			m_row++;
			m_it++;
			m_col = 0;
		}
		else if (m_col < m_it->length()) {
			m_col++;
		}
	}
	else if (dir == HOME) {
		m_col = 0;
	}
	else if (dir == END) {
		m_col = m_it->length();
	}
}

void StudentTextEditor::del() {
	if (m_col == m_it->length()) {
		if (m_row != m_lines.size() - 1) {
			m_it++;
			string line = *m_it;
			m_it = prev(m_lines.erase(m_it));
			*m_it += line;
			getUndo()->submit(Undo::Action::JOIN, m_row, m_col, 'a');
		}
	}
	else {
		char deletedChar = m_it->at(m_col);
		m_it->erase(m_col, 1);
		getUndo()->submit(Undo::Action::DELETE, m_row, m_col, deletedChar);
	}
}

void StudentTextEditor::backspace() {
	if (m_col == 0) {
		if (m_it != m_lines.begin()) {
			string line = *m_it;
			m_it = prev(m_lines.erase(m_it));
			m_col = m_it->length();
			*m_it += line;
			m_row--;
			getUndo()->submit(Undo::Action::JOIN, m_row, m_col, 'a');
		}
	}
	else {
		char backspacedChar = m_it->at(m_col - 1);
		m_it->erase(m_col - 1, 1);
		m_col--;
		getUndo()->submit(Undo::Action::DELETE, m_row, m_col, backspacedChar);
	}
	
}

void StudentTextEditor::insert(char ch) {
	if (ch == '/t') {
		for (int i = 0; i < 4; i++) {
			m_it->insert(m_col, 1, ' ');
			m_col += 1;
		}
	}
	else {
		m_it->insert(m_col, 1, ch);
		m_col++;
	}
	getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ch);
}

void StudentTextEditor::enter() {
	string prev = m_it->substr(0, m_col);
	string next = m_it->substr(m_col);
	*m_it = prev;
	m_it++;
	m_it = m_lines.insert(m_it, next);

	getUndo()->submit(Undo::SPLIT, m_row, m_col, 'a');
	m_col = 0;
	m_row += 1;
}

void StudentTextEditor::getPos(int& row, int& col) const {
	row = m_row;
	col = m_col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
	if (startRow < 0 || startRow >= m_lines.size() || numRows < 0) return -1;

	lines.clear();
	list<string>::const_iterator it = m_lines.begin();
	advance(it, startRow);

	int count = 0;
	while (count < numRows && it != m_lines.end()) {
		lines.push_back(*it);
		count++;
		it++;
	}
}

void StudentTextEditor::undo() {
	int undoRow;
	int undoCol;
	int undoCount;
	string undoText;

	Undo::Action undoAction = getUndo()->get(undoRow, undoCol, undoCount, undoText);
	if (undoAction == Undo::Action::ERROR) {
		return;
	}
	m_it = next(m_lines.begin(), undoRow);
	m_row = undoRow;
	m_col = undoCol;

	if (undoAction == Undo::Action::INSERT) {
		m_it->insert(m_col, undoText);
		m_col += undoText.length();
	}
	else if (undoAction == Undo::Action::DELETE) {
		m_col -= undoCount;
		m_it->erase(m_col, undoCount);
	}
	else if (undoAction == Undo::Action::SPLIT) {
		string prev = m_it->substr(0, m_col);
		string next = m_it->substr(m_col);
		*m_it = prev;
		m_it++;
		m_it = m_lines.insert(m_it, next);
		m_it--;
	}
	else if (undoAction == Undo::Action::JOIN) {
		m_it++;
		string line = *m_it;
		m_it = prev(m_lines.erase(m_it));
		*m_it += line;
	}
}

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
	m_undo = undo;
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
	// TODO: Undo track
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
		if (m_col == m_it->length() - 1 && m_row != m_lines.size() - 1) {
			m_row++;
			m_it++;
			m_col = 0;
		}
		else {
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
	char deletedChar;
	if (m_col == m_it->length()) {
		if (m_row != m_lines.size() - 1) {
			string next = *(++m_it);
			m_lines.remove(*m_it);
			m_it--;
			deletedChar = (*m_it)[m_col];
			*m_it = m_it->substr(0, m_col) + next;
			m_undo->submit(Undo::Action::JOIN, m_row, m_col, deletedChar);
		}
	}
	else {
		deletedChar = (*m_it)[m_col];
		*m_it = m_it->substr(0, m_col) + m_it->substr(m_col + 1);
		m_undo->submit(Undo::Action::DELETE, m_row, m_col, deletedChar);
	}
}

void StudentTextEditor::backspace() {
	char backspacedChar;
	if (m_col == 0) {
		if (m_row != 0) {
			string prev = (*(--m_it));
			m_lines.remove(*m_it);
			m_it++;
			backspacedChar = (*m_it)[m_col - 1];
			*m_it = prev.substr(0, prev.length() - 1) + *m_it;
			m_row--;
			m_col = prev.length();
			m_undo->submit(Undo::Action::JOIN, m_row, m_col, backspacedChar);
		}
	}
	else {
		backspacedChar = (*m_it)[m_col - 1];
		*m_it = m_it->substr(0, m_col - 1) + m_it->substr(m_col);
		m_col--;
		m_undo->submit(Undo::Action::DELETE, m_row, m_col, backspacedChar);
	}
	
}

void StudentTextEditor::insert(char ch) {
	if (ch == '/t') {
		for (int i = 0; i < 4; i++) {
			insert(' ');
		}
	}
	else {
		// *m_it = m_it->substr(0, m_col) + ch + m_it->substr(m_col);
		m_it->insert(m_it->begin() + m_col, ch);
		m_col++;
	}
	m_undo->submit(Undo::Action::INSERT, m_row, m_col, ch);
}

void StudentTextEditor::enter() {
	m_undo->submit(Undo::Action::SPLIT, m_row, m_col);
	string line = *m_it;
	*m_it = line.substr(0, m_col);
	m_it++;
	m_lines.insert(m_it, line.substr(m_col));
	m_row++;
	m_col = 0;
}

void StudentTextEditor::getPos(int& row, int& col) const {
	row = m_row;
	col = m_col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
	if (startRow < 0 || startRow >= m_lines.size() || numRows < 0) return -1;
	int count = 0;
	list<string>::const_iterator it = m_lines.begin();
	while (it != m_lines.end()) {
		if (count >= startRow + numRows) break;
		if (count >= startRow) {
			lines[count] = *it;
		}
		count++;
		it++;
	}
}

void StudentTextEditor::undo() {
	int undoRow;
	int undoCol;
	int undoCount;
	string undoText;

	Undo::Action undoAction = m_undo->get(undoRow, undoCol, undoCount, undoText);
	if (undoAction == Undo::Action::ERROR) {
		return;
	}
	m_row = undoRow;
	m_col = undoCol;
	for (int i = 0; i < undoRow; i++) {
		m_it++;
	}

	if (undoAction == Undo::Action::INSERT) {
		for (char undoChar : undoText) {
			insert(undoChar);
		}
	}
	else if (undoAction == Undo::Action::DELETE) {
		for (int i = 0; i < undoCount; i++) {
			del();
		}
	}
	else if (undoAction == Undo::Action::SPLIT) {
		enter();
	}
	else if (undoAction == Undo::Action::JOIN) {
		string next = *(++m_it);
		m_lines.remove(*m_it);
		m_it--;
		*m_it = *m_it + next;
	}
}

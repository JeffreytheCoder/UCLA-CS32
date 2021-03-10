#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_

#include <list>
#include <string>

#include "TextEditor.h"
#include "StudentUndo.h"

using namespace std;

class StudentTextEditor : public TextEditor {
public:


	StudentTextEditor(Undo* undo);
	~StudentTextEditor();
	bool load(std::string file);
	bool save(std::string file);
	void reset();
	void move(Dir dir);
	void del();
	void backspace();
	void insert(char ch);
	void enter();
	void getPos(int& row, int& col) const;
	int getLines(int startRow, int numRows, std::vector<std::string>& lines) const;
	void undo();

private:
	list<string> m_lines;
	list<string>::iterator m_it;
	int m_col;
	int m_row;
};

#endif // STUDENTTEXTEDITOR_H_

#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include <stack>

#include "Undo.h"

using namespace std;

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
	struct Operation {
		Action action;
		int row;
		int col;
		string text;
	};
	stack<Operation*> m_stack;
};

#endif // STUDENTUNDO_H_

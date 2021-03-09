#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
	Operation* op = new Operation;
	op->action = action;
	op->row = row;
	op->col = col;
	op->text.push_back(ch);

	while (!m_stack.empty()) {
		Operation* top = m_stack.top();
		// Only same DELETE and INSERT function counts
		// Spliting lines doesn't count
		if (top->action == action && (top->action == DELETE || top->action == INSERT) &&
			(top->col == col - 1 || top->col == col || top->col == col + 1)) {
			op->text += top->text;
			m_stack.pop();
		}
		else {
			break;
		}
	}
	m_stack.push(op);
}

StudentUndo::Action StudentUndo::get(int &row, int &col, int& count, std::string& text) {
	if (m_stack.empty()) {
		return Action::ERROR;
	}

	Operation* top = m_stack.top();
	m_stack.pop();

	count = 1;
	row = top->row;
	col = top->col;
	text = "";

	if (top->action == INSERT) {
		count = text.size();
		return DELETE;
	}
	else if (top->action == DELETE) {
		text = top->text;
		return INSERT;
	}
	else if (top->action == SPLIT) {
		return JOIN;
	}
	else if (top->action == JOIN) {
		return SPLIT;
	}
}

void StudentUndo::clear() {
	while (!m_stack.empty()) {
		delete m_stack.top();
		m_stack.pop();
	}
}

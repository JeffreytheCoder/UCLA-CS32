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

	if (m_stack.empty()) {
		m_stack.push(op);
		return;
	}
	Operation* top = m_stack.top();
	if (op->action == DELETE && top->action == DELETE && op->row == top->row) {
		if (op->col == top->col) {  // del
			op->text = top->text + op->text;
		}
		else if (op->col = top->col - 1) {  // backspace
			op->text += top->text;
		}
		m_stack.pop();
		delete top;
	}
	else if (op->action == INSERT && top->action == INSERT && op->row == top->row) {
		op->text += top->text;
		m_stack.pop();
		delete top;
	}
	m_stack.push(op);
}

StudentUndo::Action StudentUndo::get(int &row, int &col, int& count, std::string& text) {
	if (m_stack.empty()) {
		return Action::ERROR;
	}

	Operation* top = m_stack.top();
	m_stack.pop();

	row = top->row;
	col = top->col;
	count = 1;
	text = "";

	if (top->action == INSERT) {
		count = top->text.length();
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
	delete top;
	return Action::ERROR;
}

void StudentUndo::clear() {
	while (!m_stack.empty()) {
		delete m_stack.top();
		m_stack.pop();
	}
}

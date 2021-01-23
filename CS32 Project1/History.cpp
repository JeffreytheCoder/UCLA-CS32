#include <iostream>
#include <cstdlib>

#include "History.h"
#include "globals.h"

using namespace std;

History::History(int nRows, int nCols) 
	: m_rows(nRows), m_cols(nCols)
{
	for (int r = 0; r < m_rows; r++) {
		for (int c = 0; c < m_cols; c++) {
			m_record[r][c] = '.';
		}
	}
}

bool History::record(int r, int c) {
	if (r < 0 || r > MAXROWS || c < 0 || c > MAXCOLS) {
		return false;
	}
	else {
		char historyUnit = m_record[r - 1][c - 1];
		switch (historyUnit) {
		case '.': historyUnit = 'A'; break;
		case 'Z': break;
		default:
			static_cast<char>(historyUnit++); break;
		}
		m_record[r - 1][c - 1] = historyUnit;
		return true;
	}
}

void History::display() const {
	clearScreen();
	for (int r = 0; r < m_rows; r++)
	{
		for (int c = 0; c < m_cols; c++)
			cout << m_record[r][c];
		cout << endl;
	}
	cout << endl;
}

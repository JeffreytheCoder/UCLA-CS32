#include <queue>
#include <string>
using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_row(rr), m_col(cc) {}
    int r() const { return m_row; }
    int c() const { return m_col; }
private:
    int m_row;
    int m_col;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    // Return true if there is a path from (sr,sc) to (er,ec)
    // through the maze; return false otherwise

    queue<Coord> coordStack;
    Coord top(sr, sc);
    coordStack.push(top);
    maze[top.r()][top.c()] = '#';

    while (!coordStack.empty()) {
        Coord cur = coordStack.front();
        coordStack.pop();
        if (cur.r() == er && cur.c() == ec)
            return true;
        if (maze[cur.r()][cur.c() - 1] == '.') {   // west
            maze[cur.r()][cur.c() - 1] = '#';
            Coord open(cur.r(), cur.c() - 1);
            coordStack.push(open);
        }
        if (maze[cur.r()][cur.c() + 1] == '.') {   // east
            maze[cur.r()][cur.c() + 1] = '#';
            Coord open(cur.r(), cur.c() + 1);
            coordStack.push(open);
        }
        if (maze[cur.r() - 1][cur.c()] == '.') {   // north
            maze[cur.r() - 1][cur.c()] = '#';
            Coord open(cur.r() - 1, cur.c());
            coordStack.push(open);
        }
        if (maze[cur.r() + 1][cur.c()] == '.') {   // south
            maze[cur.r() + 1][cur.c()] = '#';
            Coord open(cur.r() + 1, cur.c());
            coordStack.push(open);
        }
    }
    return false;
}
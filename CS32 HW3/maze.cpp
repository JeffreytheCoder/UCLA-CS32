#include <string>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    // Return true if there is a path from (sr,sc) to (er,ec)
    // through the maze; return false otherwise

    // If the start location hits wall, return false
    if (maze[sr][sc] == 'X')
        return false;

    //If the start location is equal to the ending location, then we've
    //solved the maze, so return true.
    if (sr == er && sc == ec)
        return true;

    //For each of the four directions,
    //If the location one step in that direction(from the start
    //	location) is unvisited, and there is a path starting from that
    //	location(and ending at the same ending location as in the
    //		current call).
    //	then return true.
    maze[sr][sc] = 'X';
    if (pathExists(maze, nRows, nCols, sr + 1, sc, er, ec) ||
        pathExists(maze, nRows, nCols, sr - 1, sc, er, ec) ||
        pathExists(maze, nRows, nCols, sr, sc + 1, er, ec) ||
        pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
        return true;

    //	Return false.
    return false;
}
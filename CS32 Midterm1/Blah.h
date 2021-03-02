#include <iostream>
#include <cassert>
using namespace std;

class Num
{
public:
    Num(int i) : m_i(i) {}
    int geti() const { return m_i; }
private:
    int m_i;
};

class Blah
{
public:
    Blah(int i);
    ~Blah();
    Blah(const Blah& other);
private:
    Num* m_num;
    int m_ii;
    int* m_eyes;
};
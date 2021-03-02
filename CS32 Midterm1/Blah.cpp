#include "Blah.h"

Blah::Blah(int i)
{
    m_num = new Num(i);
    m_ii = 2 * i;
    m_eyes = &m_ii;
}

Blah::~Blah() {
    delete m_num;
}

Blah::Blah(const Blah& other) {
    m_ii = other.m_ii;
    m_eyes = &m_ii;
    m_num = new Num(other.m_num->geti());
}
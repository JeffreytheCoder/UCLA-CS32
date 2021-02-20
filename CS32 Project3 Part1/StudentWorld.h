#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class GhostRacer;

class StudentWorld : public GameWorld {
public:
    StudentWorld(string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    GhostRacer* getRacer() const;

private:
    GhostRacer* m_racer;
    vector<Actor*> m_actors;
    double m_lastYPos;
    void m_drawBorder(bool isYellow, double y);
};

#endif // STUDENTWORLD_H_
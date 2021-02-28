#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

using namespace std;

enum ActorType{ 
    HUMAN_PEDESTRAIN, 
    ZOMBIE_PEDESTRAIN, 
    ZOMBIE_CAB, 
    OIL_SLICK,
    HEAL_GOODIE,
    HOLYWATER_GOODIE,
    SOUL_GOODIE
};

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

    int getLane(Actor* actor) const;
    int getSoul2Save() const;
    void setSoul2Save(int soulCount);
    int getBonus() const;
    void setBonus(int bonus);
    GhostRacer* getRacer() const;
    vector<Actor*> getActors() const;
    void addProjectile(double x, double y, double dir);

    void spawn(ActorType actor);
    void dropActor(ActorType type, double x, double y);
    Actor* getClosestActor(int lane, double y);
    bool isOilSlickOverlap(double x, double y);

private:
    GhostRacer* m_racer;
    vector<Actor*> m_actors;
    double m_lastYPos;
    int m_soul2Save;
    void m_drawBorder(bool isYellow, double y);
    int m_bonus;
};

#endif // STUDENTWORLD_H_
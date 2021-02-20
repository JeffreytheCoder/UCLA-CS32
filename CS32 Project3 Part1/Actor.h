#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
public:
    Actor(
        StudentWorld* world,
        int imageID,
        double x,
        double y, 
        int dir = 0,
        double size = 1.0,
        int depth = 0,
        int verticalSpeed = 0,
        int horizontalSpeed = 0,
        int hp = 0,
        bool alive = true
    );
    virtual void doSomething() = 0;
    StudentWorld* getWorld();
    bool getAlive();
    int getHP();
    int getHorizontalSpeed();
    int getVerticalSpeed();
    void setAlive(bool newAlive);
    void setHP(int hitPoints);
    void setHorizontalSpeed(int speed);
    void setVerticalSpeed(int speed);

protected:
    bool isInScreen();

private:
    StudentWorld* m_world;
    bool m_alive;
    int m_verticalSpeed;
    int m_horizontalSpeed;
    int m_hp;
};

class GhostRacer : public Actor {
public:
    GhostRacer(StudentWorld* world);
    void doSomething();

private:
    int m_holyWater;
};

class BorderLine : public Actor {
public:
    BorderLine(
        StudentWorld* world,
        int borderLineColor, 
        double x, 
        double y
    );
    void doSomething();
};

#endif // ACTOR_H_
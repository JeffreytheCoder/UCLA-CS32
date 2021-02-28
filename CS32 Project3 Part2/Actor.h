#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

/*
Design of Actor Inheritance Hierarchy:
GraphObject
    Actor
        BorderLine
        HolyWaterProjectile
        Agent
            GhostRacer
            Pedestrian
                HumanPedestrian
                ZombiePedestrian
            ZombieCab
        GhostRacerActivatedObject
            OilSlick
            HealingGoodie
            HolyWaterGoodie
            SoulGoodie
 */

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
        bool alive = true,
        bool avoidanceWorthy = false
    );
    virtual void doSomething() = 0;
    StudentWorld* getWorld() const;
    bool getAlive() const;
    int getHorizontalSpeed() const;
    int getVerticalSpeed() const;
    bool getAvoidanceWorthy() const;
    void setAlive(bool newAlive);
    void setHorizontalSpeed(int speed);
    void setVerticalSpeed(int speed);
    void move(int horizontal_speed);
    bool isInScreen() const;
    bool isOverlap(Actor* other) const;
    bool isOverlap(double x, double y, double radius) const;
    virtual bool getHit(int hp);
    bool isOilSlick();

private:
    StudentWorld* m_world;
    int m_verticalSpeed;
    int m_horizontalSpeed;
    bool m_alive;
    bool m_avoidanceWorthy;
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

class HolyWaterProjectile : public Actor {
public:
    HolyWaterProjectile(
        StudentWorld* game,
        double startX,
        double startY,
        double dir
    );
    void doSomething();

private:
    int m_distance;
};


class Agent : public Actor {
public:
    Agent(
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
        bool alive = true,
        bool avoidanceWorthy = true
    );
    virtual void doSomething() = 0;
    int getHP();
    int getMovementPlan() const;
    void setHP(int hp);
    void setMovementPlan(int newMovementPlan);
private:
    int m_hp;
    int m_movementPlan;
};

class GhostRacer : public Agent {
public:
    GhostRacer(StudentWorld* world);
    void doSomething();
    int getHolyWater() const;
    void setHolyWater(int holyWater);
    virtual bool getHit(int hp);
    void getHealed(int hp);
    void spin();

private:
    int m_holyWater;
};

class Pedestrain : public Agent {
public:
    Pedestrain(
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
        bool alive = true,
        bool avoidanceWorthy = true  // pedestrain is an avoidance worthy actor
    );
    virtual void doSomething();
};

class HumanPedestrain : public Pedestrain {
public:
    HumanPedestrain(
        StudentWorld* world,
        double x, 
        double y
    );
    void doSomething();
    bool getHit(int hp);
};

class ZombiePedestrain : public Pedestrain {
public:
    ZombiePedestrain(
        StudentWorld* world,
        double x, 
        double y
    );
    void doSomething();
    int getGruntTick();
    void setGruntTick(int gruntTick);
    bool getHit(int hp);

private:
    int m_gruntTick;
};

class ZombieCab : public Agent {
public:
    ZombieCab(
        StudentWorld* world,
        double x,
        double y,
        double verticalSpeed
    );
    void doSomething();
    bool getHit(int hp);

private:
    bool m_damaged;
};

class ActivatedObject : public Actor{
  public:
    ActivatedObject(
        StudentWorld * world,
        int imageID, 
        double x,
        double y,
        int dir = 0,
        double size = 1.0,
        int depth = 0,
        double verticalSpeed = 0,
        double horizontalSpeed = 0
    );
    virtual void doSomething();
};

class OilSlick : public ActivatedObject {
public:
    OilSlick(
        StudentWorld* gameWorld,
        double x,
        double y,
        double size
    );
    void doSomething();
    bool isOilSlick();
};

class HealGoodie : public ActivatedObject {
public:
    HealGoodie(
        StudentWorld* world,
        double x, 
        double y
    );
    void doSomething();
    bool getHit(int hp);
};

class HolyWaterGoodie : public ActivatedObject {
public:
    HolyWaterGoodie(
        StudentWorld* world,
        double x,
        double y
    );
    void doSomething();
    bool getHit(int hp);
};

class SoulGoodie : public ActivatedObject {
public:
    SoulGoodie(
        StudentWorld* world,
        double x,
        double y
    );
    void doSomething();
};

#endif
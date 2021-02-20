#include "Actor.h"
#include "GameWorld.h"
#include <iostream>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(
    StudentWorld* world,
    int imageID,
    double x,
    double y,
    int dir,
    double size,
    int depth,
    int verticalSpeed,
    int horizontalSpeed,
    int hp,
    bool alive) 
    : GraphObject(imageID, x, y, dir, size, depth) 
{
    m_world = world;
    m_alive = alive;
    m_verticalSpeed = verticalSpeed;
    m_horizontalSpeed = horizontalSpeed;
    m_hp = hp;
}

StudentWorld* Actor::getWorld() {
    return m_world;
}

bool Actor::getAlive() {
    return m_alive;
}

int Actor::getHorizontalSpeed() {
    return m_horizontalSpeed;
}

int Actor::getVerticalSpeed() {
    return m_verticalSpeed;
}

int Actor::getHP() {
    return m_hp;
}

void Actor::setAlive(bool newAlive) {
    m_alive = newAlive;
}

void Actor::setHorizontalSpeed(int newSpeed) {
    m_horizontalSpeed = newSpeed;
}

void Actor::setVerticalSpeed(int newSpeed) {
    m_verticalSpeed = newSpeed;
}

void Actor::setHP(int newHitPoints) {
    m_hp = newHitPoints;
}

bool Actor::isInScreen() {
    return (getX() <= VIEW_WIDTH && getY() <= VIEW_HEIGHT);
}

// Create a ghostracer as an actor with imageID = 90, at x=128, y=32, direction at 90, size of 4, hp at 100
// Also as a specific actor with holyWater = 10
GhostRacer::GhostRacer(StudentWorld* gameWorld)
    : Actor(gameWorld, IID_GHOST_RACER, 128, 32, 90, 4.0, 0, 0, 0, 100, true) {
    m_holyWater = 10;
};

void GhostRacer::doSomething() {
    if (getHP() <= 0) {
        setAlive(false);
        return;
    }
    if (getX() <= 0) {
        if (getDirection() > 90)
            setHP(getHP() - 10);
        setDirection(82);
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
    }
    if (getX() >= (double) VIEW_WIDTH - 1) {
        if (getDirection() < 90)
            setHP(getHP() - 10);
        setDirection(98);
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
    }

    int key;
    if (getWorld()->getKey(key)) {
        switch (key) {
        case KEY_PRESS_SPACE:
            // Spray a holy water
            break;
        case KEY_PRESS_UP:
            if (getVerticalSpeed() < 5)
                setVerticalSpeed(getVerticalSpeed() + 1);
            break;
        case KEY_PRESS_DOWN:
            if (getVerticalSpeed() > -1)
                setVerticalSpeed(getVerticalSpeed() - 1);
            break;
        case KEY_PRESS_LEFT:
            if (getDirection() < 114)
                setDirection(getDirection() + 8);
            break;
        case KEY_PRESS_RIGHT:
            if (getDirection() > 66)
                setDirection(getDirection() - 8);
            break;
        }
    }
    // Move racer according to moving algorithm
    const double PI = 3.1415926;
    const double MAX_SHIFT_PER_TICK = 4.0;
    double delta_x = cos(getDirection() * PI / 180.0) * MAX_SHIFT_PER_TICK;
    moveTo(getX() + delta_x, getY());
}

// Create borderline with imageID and ghostracer, at x and y, with size = 2.0, depth = 2, verticalSpeed = -4, and hp = 1
BorderLine::BorderLine(StudentWorld* world, int imageID, double x, double y) 
    : Actor(world, imageID, x, y, 0, 2.0, 2, -4, 0, 1, true) {
}

void BorderLine::doSomething() {
    int vertical_speed = getVerticalSpeed() - getWorld()->getRacer()->getVerticalSpeed();
    int horizontal_speed = getHorizontalSpeed();
    int new_y = getY() + vertical_speed;
    int new_x = getX() + horizontal_speed;
    moveTo(new_x, new_y);
    if (!isInScreen()) {
        setAlive(false);
        return;
    }
}
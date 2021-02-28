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
    bool alive,
    bool avoidanceWorthy) 
    : GraphObject(imageID, x, y, dir, size, depth) 
{
    m_world = world;
    m_alive = alive;
    m_verticalSpeed = verticalSpeed;
    m_horizontalSpeed = horizontalSpeed;
    m_avoidanceWorthy = avoidanceWorthy;
}

StudentWorld* Actor::getWorld() const {
    return m_world;
}

bool Actor::getAlive() const {
    return m_alive;
}

int Actor::getHorizontalSpeed() const {
    return m_horizontalSpeed;
}

int Actor::getVerticalSpeed() const {
    return m_verticalSpeed;
}

bool Actor::getAvoidanceWorthy() const {
    return m_avoidanceWorthy;
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

bool Actor::isOilSlick() {
    return false;
}

void Actor::move(int horizontal_speed) {
    int vertical_speed = this->getVerticalSpeed() - getWorld()->getRacer()->getVerticalSpeed();
    int new_y = this->getY() + vertical_speed;
    int new_x = this->getX() + horizontal_speed;
    moveTo(new_x, new_y);
}

bool Actor::isInScreen() const {
    return (getX() >= 0 && getX() <= VIEW_WIDTH && getY() >= 0 && getY() <= VIEW_HEIGHT);
}

bool Actor::isOverlap(Actor* other = nullptr) const {
    if (other == nullptr)
        other = getWorld()->getRacer();  // default to compare overlap with ghostracer

    double delta_x = abs(getX() - other->getX());
    double delta_y = abs(getY() - other->getY());
    double radius_sum = getRadius() + other->getRadius();

    return delta_x < 0.25 * radius_sum && delta_y < 0.6 * radius_sum;
}

bool Actor::isOverlap(double x, double y, double radius) const {
    double delta_x = abs(this->getX() - x);
    double delta_y = abs(this->getY() - y);
    double radius_sum = this->getRadius() + radius;
    return delta_x < 0.25 * radius_sum && delta_y < 0.6 * radius_sum;
}

bool Actor::getHit(int hp) {
    return false;  // for damagable m_actors to rewrite
}

// Create borderline with imageID and ghostracer, at x and y, with size = 2.0, depth = 2, verticalSpeed = -4, and hp = 1
BorderLine::BorderLine(StudentWorld* world, int imageID, double x, double y)
    : Actor(world, imageID, x, y, 0, 2.0, 2, -4, 0, true) {
}

void BorderLine::doSomething() {
    move(0);
    if (!isInScreen()) {
        setAlive(false);
        return;
    }
}

HolyWaterProjectile::HolyWaterProjectile(
    StudentWorld* world,
    double x,
    double y,
    double dir
) : Actor(world, IID_HOLY_WATER_PROJECTILE, x, y, dir, 1.0, 1, 0, 0, true, false) {
    m_distance = 0;
}

void HolyWaterProjectile::doSomething() {
    if (!getAlive())
        return;
    vector<Actor*> m_actors = getWorld()->getActors();
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i] != this && isOverlap(m_actors[i]) && m_actors[i]->getHit(1)) {
            setAlive(false);
            return;
        }
    }
    moveForward(SPRITE_HEIGHT);  // from GraphObject
    m_distance += SPRITE_HEIGHT;
    if (!isInScreen()) {
        setAlive(false);
        return;
    }
    if (m_distance >= 160)
        setAlive(false);
}

Agent::Agent(
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
    bool alive,
    bool avoidanceWorthy)
    : Actor(world, imageID, x, y, dir, size, depth, verticalSpeed, horizontalSpeed, alive, avoidanceWorthy)
{
    m_movementPlan = 0;
    m_hp = hp;
}

int Agent::getHP() {
    return m_hp;
}

void Agent::setHP(int hp) {
    m_hp = hp;
}

int Agent::getMovementPlan() const {
    return m_movementPlan;
}

void Agent::setMovementPlan(int newMovementPlan) {
    m_movementPlan = newMovementPlan;
}

// Create a ghostracer as an actor with imageID = 90, at x=128, y=32, direction at 90, size of 4, hp at 100
// Also as a specific actor with holyWater = 10
GhostRacer::GhostRacer(StudentWorld* gameWorld)
    : Agent(gameWorld, IID_GHOST_RACER, 128, 32, 90, 4.0, 0, 0, 0, 100, true, true) {
    m_holyWater = 10;
};

void GhostRacer::doSomething() {
    if (getHP() <= 0) {
        setAlive(false);
        return;
    }
    double leftBorder = (double) ROAD_CENTER - ROAD_WIDTH / 2;
    double rightBorder = (double) ROAD_CENTER + ROAD_WIDTH / 2;
    if (getX() <= leftBorder) {
        if (getDirection() > 90)
            setHP(getHP() - 10);
        setDirection(82);
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
    }
    if (getX() >= rightBorder) {
        if (getDirection() < 90)
            setHP(getHP() - 10);
        setDirection(98);
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
    }

    int key;
    if (getWorld()->getKey(key)) {
        switch (key) {
        case KEY_PRESS_SPACE:
            if (getHolyWater() > 0) {
                const double PI = 3.1415926;
                double delta_x = SPRITE_HEIGHT * sin((double) getDirection() / 360 * 2 * PI);
                double delta_y = SPRITE_HEIGHT * cos((double) getDirection() / 360 * 2 * PI);
                getWorld()->addProjectile(getX() + delta_x, getY() + delta_y, getDirection());
                setHolyWater(getHolyWater() - 1);
            }
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

bool GhostRacer::getHit(int hp) {
    setHP(getHP() - hp);
    if (getHP() < 0) {
        setAlive(false);
        getWorld()->playSound(SOUND_PLAYER_DIE);
    }
    return true;
}

void GhostRacer::getHealed(int hp) {
    setHP(getHP() + hp);
    if (getHP() > 100)
        setHP(100);
}

void GhostRacer::spin() {
    int randomDirection;
    if (randInt(0, 1) == 0)
        randomDirection = min(getDirection() + randInt(5, 20), 120);
    else
        randomDirection = max(getDirection() - randInt(5, 20), 60);
    setDirection(randomDirection);
}

int GhostRacer::getHolyWater() const {
    return m_holyWater;
}

void GhostRacer::setHolyWater(int newHolyWater) {
    m_holyWater = newHolyWater;
}

Pedestrain::Pedestrain(
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
    bool alive,
    bool avoidanceWorthy)
    : Agent(world, imageID, x, y, 0, size, 0, -4, 0, 2, true, true) {
        
}

void Pedestrain::doSomething() {
    move(getHorizontalSpeed());
    if (!isInScreen()) {
        setAlive(false);
        return;
    }
    setMovementPlan(getMovementPlan() - 1);
    if (getMovementPlan() > 0)
        return;

    int randomHorizontalSpeed = 0;
    while (randomHorizontalSpeed == 0)  // make sure randomSpeed != 0
        randomHorizontalSpeed = randInt(-3, 3);
    setHorizontalSpeed(randomHorizontalSpeed);

    setMovementPlan(randInt(4, 32));
    if (getHorizontalSpeed() < 0)
        setDirection(180);
    else
        setDirection(0);
}

HumanPedestrain::HumanPedestrain(
    StudentWorld* world,
    double x, 
    double y) 
    : Pedestrain(world, IID_HUMAN_PED, x, y, 0, 2.0) {

}

void HumanPedestrain::doSomething() {
    if (!getAlive())
        return;
    if (isOverlap()) {
        getWorld()->getRacer()->setAlive(false);
        return;
    }
    Pedestrain::doSomething();  // move human pedestrain
}

bool HumanPedestrain::getHit(int hp) {
    // ignore hp damage since holy water doesn't injure human pedestrain
    setHorizontalSpeed(-getHorizontalSpeed());
    setDirection((getDirection() + 180) % 180);
    getWorld()->playSound(SOUND_PED_HURT);
    return true;
}

ZombiePedestrain::ZombiePedestrain(
    StudentWorld* world,
    double x,
    double y)
    : Pedestrain(world, IID_ZOMBIE_PED, x, y, 0, 3.0) {
    m_gruntTick = 0;
}

int ZombiePedestrain::getGruntTick() {
    return m_gruntTick;
}

void ZombiePedestrain::setGruntTick(int gruntTick) {
    m_gruntTick = gruntTick;
}

void ZombiePedestrain::doSomething() {
    if (!getAlive()) {
        return;
    }

    if (isOverlap()) {
        getWorld()->getRacer()->setHP(getWorld()->getRacer()->getHP() - 5);
        setHP(getHP() - 2);
        if (getHP() <= 0) {
            setAlive(false);
            getWorld()->playSound(SOUND_PED_DIE);
        }
        else {
            getWorld()->playSound(SOUND_PED_HURT);
        }
        return;
    }

    if (abs(getX() - getWorld()->getRacer()->getX()) <= 30 &&
        getY() > getWorld()->getRacer()->getY()) {
        setDirection(270);
        if (getX() < getWorld()->getRacer()->getX())
            setHorizontalSpeed(1);
        else if (getX() > getWorld()->getRacer()->getX())
            setHorizontalSpeed(-1);
        else
            setHorizontalSpeed(0);

        setGruntTick(getGruntTick() - 1);
        if (getGruntTick() <= 0) {
            getWorld()->playSound(SOUND_ZOMBIE_ATTACK);
            setGruntTick(20);
        }
    }
    Pedestrain::doSomething();  // move zombie pedestrain
}

bool ZombiePedestrain::getHit(int hp) {
    setHP(getHP() - 1);
    if (getHP() <= 0) {

        setAlive(false);
        getWorld()->playSound(SOUND_PED_DIE);

        int dropChance = randInt(0, 4);
        if (dropChance == 0)
            getWorld()->dropActor(HEAL_GOODIE, getX(), getY());
        getWorld()->increaseScore(150);
    }
    else
        getWorld()->playSound(SOUND_PED_HURT);
    return true;
}

ZombieCab::ZombieCab(
    StudentWorld* world,
    double x, 
    double y,
    double verticalSpeed)
    : Agent(world, IID_ZOMBIE_CAB, x, y, 90, 4.0, 0, verticalSpeed, 0, 3) {
    m_damaged = false;
}

void ZombieCab::doSomething() {
    if (!getAlive()) {
        return;
    }
    if (isOverlap() && !m_damaged) {
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
        getWorld()->getRacer()->getHit(20);

        if (getX() <= getWorld()->getRacer()->getX()) {
            setHorizontalSpeed(-5);
            setDirection(60 - randInt(0, 19));
        }
        else {
            setHorizontalSpeed(5);
            setDirection(120 + randInt(0, 19));
        }
        m_damaged = true;
    }
    move(getHorizontalSpeed());
    if (!isInScreen()) {
        setAlive(false);
        return;
    }

    vector<Actor*> m_actors = getWorld()->getActors();
    int cab_line = getWorld()->getLane(this);
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i]->getAvoidanceWorthy() &&
            getWorld()->getLane(m_actors[i]) == cab_line &&
            m_actors[i] != getWorld()->getRacer()) {

            if (getVerticalSpeed() >= getWorld()->getRacer()->getVerticalSpeed()) {
                if (m_actors[i]->getY() > getY() && m_actors[i]->getY() - getY() < 96) {
                    setVerticalSpeed(getVerticalSpeed() - 0.5);
                    return;
                }
            }
            else {
                if (getY() > m_actors[i]->getY() && getY() > m_actors[i]->getY() < 96) {
                    setVerticalSpeed(getVerticalSpeed() + 0.5);
                    return;
                }
            }
        }
    }
    setMovementPlan(getMovementPlan() - 1);
    if (getMovementPlan() > 0)
        return;
    setMovementPlan(randInt(4, 32));
    setVerticalSpeed(getVerticalSpeed() + randInt(-2, 2));
}

bool ZombieCab::getHit(int hp) {
    setHP(getHP() - 1);
    if (getHP() <= 0)
    {
        setAlive(false);
        getWorld()->playSound(SOUND_VEHICLE_DIE);
        int dropChance = randInt(0, 4);
        if (dropChance == 0)
        {
            getWorld()->dropActor(OIL_SLICK, getX(), getY());
        }
        getWorld()->increaseScore(200);
        return true;
    }
    getWorld()->playSound(SOUND_VEHICLE_HURT);
    return true;
}

ActivatedObject::ActivatedObject(
    StudentWorld* world,
    int imageID,
    double x,
    double y,
    int dir,
    double size,
    int depth,
    double verticalSpeed,
    double horizontalSpeed)
    : Actor(world, imageID, x, y, dir, size, depth, verticalSpeed, horizontalSpeed, true, false) {
        
};

void ActivatedObject::doSomething() {
    move(0);
    if (!isInScreen()) {
        setAlive(false);
        return;
    }
}

OilSlick::OilSlick(
    StudentWorld* world,
    double x,
    double y,
    double size
) : ActivatedObject(world, IID_OIL_SLICK, x, y, 0, size, 2, -4, 0) {

}

void OilSlick::doSomething() {
    ActivatedObject::doSomething(); // move oilslick down
    if (isOverlap()) {
        getWorld()->playSound(SOUND_OIL_SLICK);
        getWorld()->getRacer()->spin();
    }
}

bool OilSlick::isOilSlick() {
    return true;
}

HealGoodie::HealGoodie(
    StudentWorld* world,
    double x,
    double y)
    : ActivatedObject(world, IID_HEAL_GOODIE, x, y, 0, 1.0, 2, -4, 0) {

}

void HealGoodie::doSomething() {
    ActivatedObject::doSomething(); // move heal goodie down
    if (isOverlap()) {
        getWorld()->getRacer()->getHealed(10);
        setAlive(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(250);
    }
}

bool HealGoodie::getHit(int hp) {
    return true;
}

HolyWaterGoodie::HolyWaterGoodie(
    StudentWorld* world,
    double x,
    double y)
    : ActivatedObject(world, IID_HOLY_WATER_GOODIE, x, y, 90, 2.0, 2, -4, 0) {

}

void HolyWaterGoodie::doSomething() {
    ActivatedObject::doSomething(); // move holy water goodie down
    if (isOverlap()) {
        getWorld()->getRacer()->setHolyWater(getWorld()->getRacer()->getHolyWater() + 10);
        setAlive(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(50);
    }
}

bool HolyWaterGoodie::getHit(int hp) {
    return true;
}

SoulGoodie::SoulGoodie(
    StudentWorld* world,
    double x,
    double y)
    : ActivatedObject(world, IID_SOUL_GOODIE, x, y, 0, 4.0, 2, -4, 0) {

}

void SoulGoodie::doSomething() {
    ActivatedObject::doSomething(); // move soul goodie down
    if (isOverlap()) {
        getWorld()->setSoul2Save(getWorld()->getSoul2Save() - 1);
        setAlive(false);
        getWorld()->playSound(SOUND_GOT_SOUL);
        getWorld()->increaseScore(100);
    }
    setDirection((getDirection() + 350) % 360);
}
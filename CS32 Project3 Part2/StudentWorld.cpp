#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

GameWorld* createStudentWorld(string assetPath) {
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath) {
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init() {
    m_racer = new GhostRacer(this);
    m_bonus = 5000;
    m_soul2Save = getLevel() * 2 + 5;

    const double N = VIEW_HEIGHT / SPRITE_HEIGHT;
    const double M = N / 4;
    for (int i = 0; i < N; i++)
        m_drawBorder(true, (double) i * SPRITE_HEIGHT);

    for (int i = 0; i < M; i++)
        m_drawBorder(false, (double) i * 4 * SPRITE_HEIGHT);

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {

    // Draw new BorderLines
    int vertical_speed = -4 - m_racer->getVerticalSpeed();
    m_lastYPos += vertical_speed;
    double new_border_y = (double)VIEW_HEIGHT - SPRITE_HEIGHT;
    double delta_y = new_border_y - m_lastYPos;
    if (delta_y >= SPRITE_HEIGHT)
        m_drawBorder(true, new_border_y);
    if (delta_y >= (double)4 * SPRITE_HEIGHT)
        m_drawBorder(false, new_border_y);

    // Spawn actors
    spawn(HUMAN_PEDESTRAIN);
    spawn(ZOMBIE_PEDESTRAIN);
    spawn(ZOMBIE_CAB);
    spawn(OIL_SLICK);
    spawn(HOLYWATER_GOODIE);
    spawn(SOUL_GOODIE);

    // dosomething
    getRacer()->doSomething();
    if (!getRacer()->getAlive()) {
        decLives();
        playSound(SOUND_PLAYER_DIE);
        return GWSTATUS_PLAYER_DIED;
    }
    for (Actor* actor : m_actors) {
        if (actor->getAlive())
            actor->doSomething();
        if (!getRacer()->getAlive())
        {
            decLives();
            playSound(SOUND_PLAYER_DIE);
            return GWSTATUS_PLAYER_DIED;
        }
        if (getSoul2Save() <= 0) // player finished the level
        {
            increaseScore(getBonus());
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }

    // Remove dead m_actors
    int i = 0;
    while (i < m_actors.size()) {
        if (!m_actors[i]->getAlive()) {
            delete m_actors[i];
            m_actors.erase(m_actors.begin() + i);
        }
        i++;
    }

    // Update game status line
    ostringstream text;
    text << "  Score: ";
    text << setw(4) << getScore();
    text << "  Lvl: ";
    text << setw(1) << getLevel();
    text << "  Souls2Save: ";
    text << setw(1) << getSoul2Save();
    text << "  Lives: ";
    text << setw(1) << getLives();
    text << "  Health: ";
    text << setw(2) << getRacer()->getHP();
    text << "  Sprays: ";
    text << setw(1) << getRacer()->getHolyWater();
    text << "  Bonus: ";
    text << setw(4) << getBonus();
    string t = text.str();
    setGameStatText(t);
    setBonus(getBonus() - 1);

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    for (int i = 0; i < m_actors.size(); i++) {
        delete m_actors[i];
    }
    m_actors.clear();
    delete m_racer;
}

int StudentWorld::getLane(Actor* actor) const {
    double leftEdge = (double) ROAD_CENTER - ROAD_WIDTH / 2;
    double rightEdge = (double) ROAD_CENTER + ROAD_WIDTH / 2;
    double firstLine = leftEdge + ROAD_WIDTH / 3;
    double secondLine = rightEdge - ROAD_WIDTH / 3;
    if (actor->getX() <= firstLine)
        return 0;
    else if (actor->getX() <= secondLine)
        return 1;
    return 2;
}

int StudentWorld::getSoul2Save() const {
    return m_soul2Save;
}

void StudentWorld::setSoul2Save(int soulCount) {
    m_soul2Save = soulCount;
}

int StudentWorld::getBonus() const {
    return m_bonus;
}

void StudentWorld::setBonus(int bonus) {
    m_bonus = bonus;
}

GhostRacer* StudentWorld::getRacer() const {
    return m_racer;
}

vector<Actor*> StudentWorld::getActors() const {
    return m_actors;
}

void StudentWorld::addProjectile(double x, double y, double dir) {
    HolyWaterProjectile* projectile = new HolyWaterProjectile(this, x, y, dir);
    playSound(SOUND_PLAYER_SPRAY);
    m_actors.push_back(projectile);
}

void StudentWorld::m_drawBorder(bool ifYellow, double y) {
    const double LEFT_EDGE = (double) ROAD_CENTER - ROAD_WIDTH / 2;
    const double RIGHT_EDGE = (double) ROAD_CENTER + ROAD_WIDTH / 2;

    if (ifYellow) {
        m_actors.push_back(new BorderLine(
            this, IID_YELLOW_BORDER_LINE, LEFT_EDGE, y
        ));
        m_actors.push_back(new BorderLine(
            this, IID_YELLOW_BORDER_LINE, RIGHT_EDGE, y
        ));
    }
    else {
        const double LEFT_X = LEFT_EDGE + ROAD_WIDTH / 3;
        const double RIGHT_X = RIGHT_EDGE - ROAD_WIDTH / 3;

        m_actors.push_back(new BorderLine(
            this, IID_WHITE_BORDER_LINE, LEFT_X, y
        ));

        m_actors.push_back(new BorderLine(
            this, IID_WHITE_BORDER_LINE, RIGHT_X, y
        ));
        m_lastYPos = y;
    }
}

void StudentWorld::spawn(ActorType actor) {
    const double LEFT_EDGE = (double) ROAD_CENTER - ROAD_WIDTH / 2;
    const double RIGHT_EDGE = (double) ROAD_CENTER + ROAD_WIDTH / 2;

    int randomChance, xPosition;
    switch (actor) {
    case HUMAN_PEDESTRAIN: {
        int chanceHumanPed = max(200 - getLevel() * 10, 30);
        randomChance = randInt(0, chanceHumanPed - 1);
        if (randomChance == 0)
            m_actors.push_back(new HumanPedestrain(this, randInt(0, VIEW_WIDTH), VIEW_HEIGHT));
        break;
    }
    case ZOMBIE_PEDESTRAIN: {
        int chanceZombiePed = max(100 - getLevel() * 10, 20);
        randomChance = randInt(0, chanceZombiePed - 1);
        if (randomChance == 0)
            m_actors.push_back(new ZombiePedestrain(this, randInt(0, VIEW_WIDTH), VIEW_HEIGHT));
        break;
    }
    case ZOMBIE_CAB: {
        int chanceZombieCab = max(100 - getLevel() * 10, 20);
        if (randInt(0, chanceZombieCab - 1) == 0) {
            double startY = -1;
            double startSpeed = -1;

            int lane = randInt(0, 2);
            int endLane = (lane + 3) % 3;
            do {
                double closest_bottom_distance = VIEW_HEIGHT + 1;
                double closest_top_distance = -1;
                if (lane == getLane(getRacer())) {
                    // GhostRacer is not contained in the m_actors vector
                    closest_bottom_distance = 32;
                    closest_top_distance = 32;
                }

                for (int i = 0; i < m_actors.size(); i++) {
                    if (!m_actors[i]->getAvoidanceWorthy() || getLane(m_actors[i]) != lane) {
                        continue;
                    }

                    if (m_actors[i]->getY() < closest_bottom_distance) {
                        closest_bottom_distance = m_actors[i]->getY();
                    }

                    if (m_actors[i]->getY() > closest_top_distance) {
                        closest_top_distance = m_actors[i]->getY();
                    }
                }

                if (closest_bottom_distance == VIEW_HEIGHT + 1 || closest_bottom_distance > VIEW_HEIGHT / 3) {
                    startY = SPRITE_HEIGHT / 2;
                    startSpeed = getRacer()->getVerticalSpeed() + randInt(2, 4);
                    break;
                }

                if (closest_top_distance == -1 || closest_top_distance < VIEW_HEIGHT * 2 / 3) {
                    startY = VIEW_HEIGHT - SPRITE_HEIGHT / 2;
                    startSpeed = getRacer()->getVerticalSpeed() - randInt(2, 4);
                    break;
                }

                lane = (lane + 1) % 3;
            } while (lane != endLane);


            if (startY != -1) {
                double startX = 0;
                if (lane == 0) {
                    startX = ROAD_CENTER - ROAD_WIDTH / 3;
                }
                else if (lane == 1) {
                    startX = ROAD_CENTER;
                }
                else if (lane == 2) {
                    startX = ROAD_CENTER + ROAD_WIDTH / 3;
                }

                // Add a zombie cab this tick
                ZombieCab* zombie_cab = new ZombieCab(this, startX, startY, startSpeed);
                m_actors.push_back(zombie_cab);
            }
        }
        break;
    }
    case OIL_SLICK: {
        int chanceOilSlick = max(150 - getLevel() * 10, 40);
        randomChance = randInt(0, chanceOilSlick - 1);
        int randomSize = randInt(2, 5);
        if (randomChance == 0)
            m_actors.push_back(new OilSlick(this, randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT, randomSize));
        break;
    }
    case HEAL_GOODIE: {
        break;
    }
    case HOLYWATER_GOODIE: {
        int chanceHolyWater = 100 + 10 * getLevel();
        randomChance = randInt(0, chanceHolyWater - 1);
        xPosition = randInt(LEFT_EDGE, RIGHT_EDGE);
        if (randomChance == 0 && !isOilSlickOverlap(xPosition, VIEW_HEIGHT))
            m_actors.push_back(new HolyWaterGoodie(this, xPosition, VIEW_HEIGHT));
        break;
    }
    case SOUL_GOODIE: {
        randomChance = randInt(0, 100 - 1);
        xPosition = randInt(LEFT_EDGE, RIGHT_EDGE);
        if (randomChance == 0 && !isOilSlickOverlap(xPosition, VIEW_HEIGHT))
            m_actors.push_back(new SoulGoodie(this, xPosition, VIEW_HEIGHT));
        break;
    }
    default:
        break;
    }
}

void StudentWorld::dropActor(ActorType type, double x, double y)
{
    if (type == HEAL_GOODIE) {
        m_actors.push_back(new HealGoodie(this, x, y));
    }
    else if (type == OIL_SLICK) {
        int randomSize = randInt(2, 5);
        m_actors.push_back(new OilSlick(this, x, y, randomSize));
    }
}

Actor* StudentWorld::getClosestActor(int lane, double y) {
    Actor* closestActor = nullptr;
    double closestDistance = 300;

    if (lane == getLane(getRacer()) 
        && abs(getRacer()->getY() - y) < closestDistance) { // check if player in the lane
        closestDistance = abs(getRacer()->getY() - y);
        closestActor = getRacer();
    }
    for (int i = 0; i < m_actors.size(); i++) { // check if other actors in the lane
        if (m_actors[i]->getAvoidanceWorthy() && 
            getLane(m_actors[i]) == lane 
            && abs(m_actors[i]->getY() - y) < closestDistance) {
            closestDistance = abs(m_actors[i]->getY() - y);
            closestActor = m_actors[i];
        }
    }
    return closestActor;
}

bool StudentWorld::isOilSlickOverlap(double x, double y) {
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i]->isOilSlick())
            return m_actors[i]->isOverlap(x, y, m_actors[i]->getRadius());
    }
    return false;
}
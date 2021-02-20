#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
#include <iostream>

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

    const double N = VIEW_HEIGHT / SPRITE_HEIGHT;
    const double M = N / 4;
    for (int i = 0; i < N; i++)
        m_drawBorder(true, (double) i * SPRITE_HEIGHT);

    for (int i = 0; i < M; i++)
        m_drawBorder(false, (double) i * 4 * SPRITE_HEIGHT);

    return GWSTATUS_CONTINUE_GAME;
}

GhostRacer* StudentWorld::getRacer() const {
    return m_racer;
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

int StudentWorld::move() {
    // Let all actors doSomething
    m_racer->doSomething();
    for (int i = 0; i < m_actors.size(); i++) {
        m_actors[i]->doSomething();
    }

    // Draw new BorderLines
    int vertical_speed = -4 - m_racer->getVerticalSpeed();
    m_lastYPos += vertical_speed;
    double new_border_y = (double) VIEW_HEIGHT - SPRITE_HEIGHT;
    double delta_y = new_border_y - m_lastYPos;
    if (delta_y >= SPRITE_HEIGHT)
        m_drawBorder(true, new_border_y);
    if (delta_y >= (double) 4 * SPRITE_HEIGHT)
        m_drawBorder(false, new_border_y);

    // Remove dead actors
    if (!m_racer->getAlive()) {
        // TODO: decrements life
        playSound(SOUND_PLAYER_DIE);
        return GWSTATUS_PLAYER_DIED;
    }
    // TODO: IF racer saved required number of souls(Level * 2 + 5), retrun GWSTATUS_FINISHED_LEVEL
    int i = 0;
    while (i < m_actors.size()) {
        if (!m_actors[i]->getAlive()) {
            delete m_actors[i];
            m_actors.erase(m_actors.begin() + i);
        }
        i++;
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    for (int i = 0; i < m_actors.size(); i++) {
        delete m_actors[i];
    }
    m_actors.clear();
    delete m_racer;
}
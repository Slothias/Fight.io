#include "GameEngine.hpp"
#include <cmath>
#define PI 3.14159265

 GameEngine::GameEngine() {
    mapSize = 2000;
    players.clear();
}
GameEngine::~GameEngine() {
    for (auto x : players) {
        delete x.second;
    }
}

GameEngine* GameEngine::GetInstance() {
    static GameEngine instance;
    return &instance;
}

std::string GameEngine::CreatePlayer(std::string name) {
    if (players.find(name) != players.end()) {
        return "Name is used!";
    }
    player* p = new player(name, 0, 0, 0);
    players[name] = p;
	return p->toString();
}

std::string GameEngine::CheckRequest(std::string name, std::string msg) {
    player* actplayer = (*(players.find(name))).second;
    std::stringstream ss(msg);
    std::string line;

    std::getline(ss,line,'|');
    int event_type = std::stoi(line);

    std::getline(ss,line,'|');
    float curx = std::stof(line);
    std::getline(ss,line,'|');
    float cury = std::stof(line);
    std::getline(ss,line,'|');
    float getrot = std::stof(line);

    switch (event_type) {
        case 0: {
            ///rotation
            actplayer->setRotation(getrot);
            return actplayer->toString();
        }
        case 1: {
            ///move
            if (curx <= -mapSize || curx >= mapSize ||
                cury <= -mapSize || cury >= mapSize)
            {
                return actplayer->toString();
            }
            actplayer->setPosition(curx,cury);
            return actplayer->toString();
        }
        case 2: {
            ///hitbox

            for(std::pair<std::string,player*> pr : players) {
                player* p = pr.second;
                float diraction = atan2( (p->getY() - actplayer->getY()) , (p->getX() - actplayer->getX()) ) * 180/PI;
               /* if(///diraction...
                   && sqrt(pow(actplayer->getX() - p->getX(), 2) + pow(actplayer->getY() - p->getY(), 2))/** < actplayer->getWeapon()->getRange() ) {

                }*/
            }
            return actplayer->toString();
            break;
        }
        case 3: {
            ///pickup
            break;
        }
    }

/**
    /// Rotation
    if (actplayer->getRot() != getrot) {
        actplayer->setRotation(getrot);
        return actplayer->toString();
    }
    /// Border
    if (actplayer->getX() != curx || actplayer->getY() != cury) {
        if (curx <= -mapSize || curx >= mapSize ||
            cury <= -mapSize || cury >= mapSize)
            {
            return actplayer->toString();
            }
        actplayer->setPosition(curx,cury);
    }
        /// Hitbox
        return actplayer->toString();
**/
}

double GameEngine::GetMapSize() {
    return mapSize;
}


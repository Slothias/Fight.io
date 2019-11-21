#include "GameEngine.hpp"

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
    float curx = std::stof(line);
    std::getline(ss,line,'|');
    float cury = std::stof(line);
    std::getline(ss,line,'|');
    float getrot = std::stof(line);
    if (actplayer->getRot() != getrot) {
        actplayer->setRotation(getrot);
        return actplayer->toString();
    }
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
}

double GameEngine::GetMapSize() {
    return mapSize;
}


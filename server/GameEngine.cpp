#include "GameEngine.hpp"
#include <vector>
#include <string>
#include <cmath>
#include<cstdlib>
#include<thread>
#define PI 3.1415

GameEngine::GameEngine(){

mapSize = 2000;
players.clear();
    for(int i = 0; i < WP_SIZE; ++i) {
        weapons[i] = new Weapon(i);
}
}
GameEngine::GameEngine(Server* s) {
    mapSize = 2000;
    srand (time(NULL));
    server = s;
    players.clear();
    for(int i = 0; i < WP_SIZE; ++i) {
        weapons[i] = new Weapon(i);
    }
}
GameEngine::~GameEngine() {
    for(auto x : p_mutexes) {
        delete x.second;
    }
    for(auto x : players) {
        delete x.second;
    }
    for(auto x : weapons) {
        delete x;
    }
    delete players_map;
    server = nullptr;
}

GameEngine* GameEngine::GetInstance(Server* s) {
    static GameEngine instance;
    return &instance;
}

std::string GameEngine::CreatePlayer(std::string name) {
    if (players.find(name) != players.end()) {
        return "Name is already used!";
    }
    float x,y = 0;
    GenerateXY(x,y);
    player* p = new player(name, x, y, 0);
    players[name] = p;
    p_mutexes[p] = new std::mutex();
    std::cout << "OKOK" << std::endl;
	return "OK";
}

void GameEngine::GenerateXY(float &x, float &y) {
    srand(time(NULL));
    x = (fmod(rand(),(GetMapSize()*2+1))-GetMapSize());
    y = (fmod(rand(),(GetMapSize()*2+1))-GetMapSize());
}

std::string GameEngine::ReSpawn(std::string name) {
    player* p = players.at(name);
    p_mutexes[p]->lock();
    if(p->getCurrentHp()==0)
    {
        float x,y,rot = 0;
        int wp=0;
        int curhp=100;
        int maxhp=100;
        int score = 0;
        GenerateXY(x,y);
        p->setPosition(x,y);
        p->setRotation(rot);
        p->setCurrentHp(curhp);
        p->setMaxHp(maxhp);
        p->setScore(score);
        p->setWeapon(wp);
        p_mutexes[p]->unlock();
        return name+":"+p->toString();
    }
    else
    {
    return name+":"+p->toString();
    }
}

std::string GameEngine::GetMe(std::string name) {
    std::string me = players[name]->toString();
    std::string ret = "1111" + me.substr(3);
    return ret;
}

std::vector<std::string> GameEngine::CheckRequest(std::string name, std::string data) {
    player* actplayer = players.at(name);
    std::vector<std::string> ret;
    if(data.find("EXIT") != std::string::npos) { ///EXIT
        delete actplayer;
        players.erase(name);
        ret.push_back(name + ":" + data);
        return ret;
    }
    std::stringstream ss(data);
    std::string flags;
    std::string line;
    actplayer->setPoke(false);
    float curx,cury,getrot;
    std::getline(ss,flags,'|');

    if(flags.at(0) == '1') ///MOVE
    {
        p_mutexes[actplayer]->lock();
        std::getline(ss,line,'|');
        curx = std::stof(line);
        std::getline(ss,line,'|');
        cury = std::stof(line);
        if (!(curx <= -mapSize || curx >= mapSize ||
            cury <= -mapSize || cury >= mapSize)) {
            actplayer->setPosition(curx,cury);
        }
        p_mutexes[actplayer]->unlock();
    }
    if(flags.at(1) == '1') ///ROTATION
    {
        p_mutexes[actplayer]->lock();
        std::getline(ss,line,'|');
        getrot = std::stof(line);
        actplayer->setRotation(getrot);
        p_mutexes[actplayer]->unlock();
    }
    std::getline(ss,line,'|');
    std::getline(ss,line,'|');
    std::getline(ss,line,'|');
    std::getline(ss,line,'|');
    int wp = std::stoi(line);
    if(wp != actplayer->getWeapon())
        actplayer->setWeapon(wp);
    if(flags.at(2) == '1') ///POKE
    {
        p_mutexes[actplayer]->lock();
        actplayer->setPoke(true);
        //std::cout<<flags.substr(0,4);
        float w_x = actplayer->getX() + cos((actplayer->getRot()-90)*3.1415/180) * weapons[actplayer->getWeapon()]->getRange();
        float w_y = actplayer->getY() + sin((actplayer->getRot()-90)*3.1415/180)* weapons[actplayer->getWeapon()]->getRange();
        players_map->lock();
        for(std::pair<std::string,player*> pr : players) {
            player* p = pr.second;
            if(p != actplayer) {
                if(sqrt(pow(w_x - p->getX(),2) + pow(w_y - p->getY(),2)) <= p->getHitboxRadius()) {
                    p->setCurrentHp(p->getCurrentHp() - weapons[actplayer->getWeapon()]->getPower());
                    if(p->getCurrentHp() <= 0) {
                        ///DIED!
                    }
                    std::cout<<"TALALAT, ALDOZAT:"<<p->getName()<<std::endl;
                    ret.push_back(p->getName() + ":" + p->toString());
                }
            }
        }
        players_map->unlock();
        p_mutexes[actplayer]->unlock();
    }
    if(flags.at(3) == '1') ///PICK UP A WEAPON
    {
        p_mutexes[actplayer]->lock();
        ///TODO

        p_mutexes[actplayer]->unlock();
    }
    p_mutexes[actplayer]->lock();
    ret.push_back(name + ":" + actplayer->toString());
    p_mutexes[actplayer]->unlock();
    return ret;

    /**curPoking=flags.at(3)=='1';
    std::getline(ss,line,'|');
    int maxhp = std::stoi(line);
    std::getline(ss,line,'|');
    int curhp = std::stoi(line);
    std::getline(ss,line,'|');
    int getscore = std::stoi(line);
    std::getline(ss,line,'|');
    int wp =std::stoi(line);
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

    switch (flags) {
        case .at(2) = 1: {
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
                if(diraction...
                   && sqrt(pow(actplayer->getX() - p->getX(), 2) + pow(actplayer->getY() - p->getY(), 2))/ < actplayer->getWeapon()->getRange() ) {

                }
            }
            return actplayer->toString();
            break;
        }
        case 3: {
            ///pickup
            break;
        }
    }
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
*/
}

double GameEngine::GetMapSize() {
    return mapSize;
}


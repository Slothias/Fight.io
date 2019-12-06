#include "GameEngine.hpp"
#include <vector>
#include <string>
#include <cmath>
#include<cstdlib>
#include<thread>
#define PI 3.14159265

 GameEngine::GameEngine() {
    mapSize = 2000;
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
}

GameEngine* GameEngine::GetInstance() {
    static GameEngine instance;
    return &instance;
}

std::string GameEngine::CreatePlayer(std::string name) {
    if (players.find(name) != players.end()) {
        return "Name is already used!";
    }
    player* p = new player(name, 0, 0, 0);
    players[name] = p;
    p_mutexes[p] = new std::mutex();
    std::cout << "OKOK" << std::endl;
	return "OK";
}

std::string GameEngine::GetMe(std::string name) {
    std::string me = players[name]->toString();
    std::string ret = "1111" + me.substr(3);
    return ret;
}
std::string GameEngine::ReSpawn(std::string name)
{
    player* p = players.at(name);
    p_mutexes[p]->lock();
    if(p->getCurrentHp()==0)
    {
        float x,y,rot = 0;
        int curhp,maxhp,score,wp=0;
        x = rand()%((int)GetMapSize())-(GetMapSize()/2);
        y = rand()%((int)GetMapSize())-(GetMapSize()/2);
        curhp=maxhp=100;
        score = 0;
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
std::vector<std::string> GameEngine::CheckRequest(std::string name, std::string data) {
    player* actplayer = players.at(name);
    std::vector<std::string> ret;
    if(data.find("EXIT") != std::string::npos) {
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

    p_mutexes[actplayer]->lock();
    if(flags.at(0) == '1') ///MOVE
    {
        std::getline(ss,line,'|');
        curx = std::stof(line);
        std::getline(ss,line,'|');
        cury = std::stof(line);
        if (!(curx <= -mapSize || curx >= mapSize ||
            cury <= -mapSize || cury >= mapSize)) {
            actplayer->setPosition(curx,cury);
        }
    }
    if(flags.at(1) == '1') ///ROTATION
    {
        std::getline(ss,line,'|');
        getrot = std::stof(line);
        actplayer->setRotation(getrot);
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
        actplayer->setPoke(true);
        //std::cout<<flags.substr(0,4);
        float w_x = actplayer->getX() + cos((actplayer->getRot()-90)*3.1415/180) * weapons[actplayer->getWeapon()]->getRange();
        float w_y = actplayer->getY() + sin((actplayer->getRot()-90)*3.1415/180)* weapons[actplayer->getWeapon()]->getRange();
        for(std::pair<std::string,player*> pr : players) {
            player* p = pr.second;
            if(p != actplayer) {
                p_mutexes[p]->lock();
                if(sqrt(pow(w_x - p->getX(),2) + pow(w_y - p->getY(),2)) <= p->getHitboxRadius()) {
                    p->setCurrentHp(p->getCurrentHp() - weapons[actplayer->getWeapon()]->getPower());
                    ret.push_back(p->getName() + ":" + p->toString());
                }
                p_mutexes[p]->unlock();
            }
        }
    }
    if(flags.at(3) == '1') ///PICK UP A WEAPON
    {
        ///TODO
    }
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


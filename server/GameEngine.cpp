#include "GameEngine.hpp"
#include <cmath>
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
        return "Name is used!";
    }
    player* p = new player(name, 0, 0, 0);
    players[name] = p;
    p_mutexes[p] = new std::mutex();
	return p->toString();
}

std::string GameEngine::CheckRequest(std::string name, std::string data) {
    player* actplayer = (*(players.find(name))).second;
    ///int event_type;
    std::stringstream ss(data);
    std::string flags;
    std::string line;
    float curx,cury,getrot;
    bool curPoking=false;
    std::getline(ss,flags,'|');

    p_mutexes[actplayer]->lock();
    if(flags.at(1) == '1') ///MOVE
    {
        std::getline(ss,line,'|');
        curx = std::stof(line);
        std::getline(ss,line,'|');
        cury = std::stof(line);
        if (curx <= -mapSize || curx >= mapSize ||
            cury <= -mapSize || cury >= mapSize)
        {
            ///return actplayer->toString();
        }
        actplayer->setPosition(curx,cury);
    }
    if(flags.at(2) == '1') ///ROTATION
    {
        std::getline(ss,line,'|');
        getrot = std::stof(line);
        actplayer->setRotation(getrot);
        ///return actplayer->toString();
    }
    if(flags.at(3) == '1') ///POKE
    {
        //std::cout<<flags.substr(0,4);
        curPoking = true;
        for(std::pair<std::string,player*> pr : players) {
            player* p = pr.second;
            if(p != actplayer) {
                p_mutexes[p]->lock();
                float range = sqrt(pow(actplayer->getX() - p->getX(), 2) + pow(actplayer->getY() - p->getY(), 2));
                float diraction = atan2( (p->getY() - actplayer->getY()) , (p->getX() - actplayer->getX()) ) * 180/PI;
                diraction += 360;
                diraction = fmod(diraction, 360);
                if(true ///diraction... actplayer->getWeapon()->getRange()
                   && range <= weapons[actplayer->getWeapon()]->getRange()) {
                    p->setCurrentHp(p->getCurrentHp() - weapons[actplayer->getWeapon()]->getPower());
                    if(p->getCurrentHp() <= 0){
                        ///Died!!
                    }
                }
                p_mutexes[p]->unlock();
            }
        }
        ///return actplayer->toString();

    }else{
        curPoking = false;
    }
    p_mutexes[actplayer]->unlock();

    return actplayer->toString(); ///nem jó még a return, mert a poke hibás...

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
*/
/**    switch (flags) {
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


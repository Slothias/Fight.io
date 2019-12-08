#include "GameEngine.hpp"
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <thread>
#include <chrono>
#define PI 3.1415

GameEngine::GameEngine(){

    maxPlayers=0;
    mapSize=0;
    readCfg();
    srand (time(NULL));
    players.clear();
    drop_weapons = std::vector<Weapon*>();
    dw_mutex = new std::mutex();
    players_map = new std::mutex();
    for(int i = 0; i < WP_SIZE; ++i) {
        weapons[i] = new Weapon(i);
    }
    thread_lifetime = true;
    std::thread w_gen_thread(&GameEngine::GenerateWeapon,*this);
    w_gen_thread.detach();
}
GameEngine::GameEngine(Server* s) {
    maxPlayers=0;
    mapSize=0;
    readCfg();
    srand (time(NULL));
    server = s;
    players.clear();
    drop_weapons = std::vector<Weapon*>();
    dw_mutex = new std::mutex();
    players_map = new std::mutex();
    for(int i = 0; i < WP_SIZE; ++i) {
        weapons[i] = new Weapon(i);
    }
    thread_lifetime = true;
    std::thread w_gen_thread(&GameEngine::GenerateWeapon,*this);
    w_gen_thread.detach();
}
GameEngine::~GameEngine() {
    thread_lifetime = false;
    for(auto &x : p_mutexes) {
        delete x.second;
    }
    for(auto &x : players) {
        delete x.second;
    }
    for(auto &x : weapons) {
        delete x;
    }
    for(auto &x : drop_weapons) {
        delete x;
    }
    delete players_map;
    delete dw_mutex;
    server = nullptr;
}

GameEngine* GameEngine::GetInstance(Server* s) {
    static GameEngine instance(s);
    return &instance;
}
void GameEngine::readCfg()
{
    std::string path= "..\\..\\..\\Includes\\config.cfg";
    std::ifstream myfile(path.c_str());
    if(myfile.is_open())
    {
        std::string line;
        myfile>>line;
        myfile>>line;
        maxPlayers = stoi(line);
        myfile>>line;
        myfile>>line;
        mapSize = stod(line);
    }
    myfile.close();
}
int GameEngine::GetMaxPlayers()
{
    return maxPlayers;
}
int GameEngine::calculateScore(player* killer,player* killed)
{
    return 0;
}
bool GameEngine::GenNotGood(const float &x, const float &y) {
    for(Weapon* w : drop_weapons) {
        if(w->getX() == x && w->getY() == y)
            return true;
    }
    return false;
}
void GameEngine::GenerateWeapon() {
    while(thread_lifetime && players.size() - 1 >= drop_weapons.size()) {
        srand(time(NULL));
        std::this_thread::sleep_for(std::chrono::seconds(30));
        float x,y;
        GenerateXY(x,y);
        std::cout<<"gener"<<std::endl;
        while(GenNotGood(x, y))
            GenerateXY(x,y);
        std::cout<<"nem talal"<<std::endl;
        int Wtype = (rand() % 100) + 1;
        if(Wtype < 10) { /// ~10% - 5ös fegyó
            Wtype = 5;
        }
        else if(10 <= Wtype && Wtype < 25) { /// ~15% - 5ös fegyó
            Wtype = 4;
        }
        else if(25 <= Wtype && Wtype < 45) { /// ~20% - 5ös fegyó
            Wtype = 3;
        }
        else if(45 <= Wtype && Wtype < 70) { /// ~25% - 5ös fegyó
            Wtype = 2;
        }
        else { /// ~30% - 5ös fegyó
            Wtype = 1;
        }
        Weapon* w = new Weapon(Wtype);
        w->setXY(x,y);
        drop_weapons.push_back(w);
        std::stringstream ss;
        ss << "Server:" << drop_weapons.size()-1 << "|" << Wtype << "|" << x << "|" << y;
        server->sendData(ss.str());
    }
}

std::string GameEngine::CreatePlayer(std::string name) {
    if (players.find(name) != players.end()) {
        return "Name is already used!";
    }
    float x,y = 0;
    GenerateXY(x,y);
    player* p = new player(name, x, y, 0);
    players[name]   = p;
    p_mutexes[p] = new std::mutex();
    std::cout << "OKOK" << std::endl;
	return "OK";
}

void GameEngine::GenerateXY(float &x, float &y) {
    srand(time(NULL));
    x = rand()%((int)GetMapSize())-(GetMapSize()/2);
    y = rand()%((int)GetMapSize())-(GetMapSize()/2);
    //x = (fmod(rand(),(GetMapSize()*2+1))-GetMapSize());
    //y = (fmod(rand(),(GetMapSize()*2+1))-GetMapSize());
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
    std::string ret = "1111" + me.substr(4);
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
        //players_map->lock();
        for(std::pair<std::string,player*> pr : players) {
            player* p = pr.second;
            if(p != actplayer) {
                p_mutexes[p]->lock();
                if(sqrt(pow(w_x - p->getX(),2) + pow(w_y - p->getY(),2)) <= p->getHitboxRadius()) {
                    p->setCurrentHp(p->getCurrentHp() - weapons[actplayer->getWeapon()]->getPower());
                    if(p->getCurrentHp() <= 0) {
                        int prevScore = actplayer->getScore();
                        int newScore=prevScore+1;
                        actplayer->setScore(newScore);
                        actplayer->setMaxHp(newScore*100);
                        }
                    std::cout<<"TALALAT, ALDOZAT:"<<p->getName()<<std::endl;
                    ret.push_back(p->getName() + ":" + p->toString());
                }
                p_mutexes[p]->unlock();
            }
        }
       // players_map->unlock();
        p_mutexes[actplayer]->unlock();
    }
    if(flags.at(3) == '1') ///PICK UP A WEAPON
    {
        p_mutexes[actplayer]->lock();
        dw_mutex->lock();
        std::vector<Weapon*> cp = drop_weapons;
        dw_mutex->unlock();
        int i = 0;
        for(Weapon* x : cp) {
            float dest = sqrt(pow(x->getX() - actplayer->getX(),2) + pow(x->getY() - actplayer->getY(),2));
            if(dest <= actplayer->getHitboxRadius()) {
                actplayer->setWeapon(x->getType());
                Weapon* w = x;
                delete w;
                drop_weapons[i] = nullptr;
            }
            ++i;
        }
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


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
    for(int i = 0; i < WP_SIZE; ++i) {
        weapons[i] = new Weapon(i);
    }
    for(int i=0; i<drop_weapons.size();i++)
    {
        drop_weapons[i] =nullptr;
    }
    thread_lifetime = true;
}
GameEngine::GameEngine(Server* s) {
    maxPlayers=0;
    mapSize=0;
    readCfg();
    srand (time(NULL));
    server = s;
    players.clear();
    drop_weapons = std::vector<Weapon*>();
    drop_weapons.resize(maxPlayers-1);
    for(int i = 0; i < WP_SIZE; ++i) {
        weapons[i] = new Weapon(i);
    }
    for(int i=0; i<drop_weapons.size();i++)
    {
        drop_weapons[i] =nullptr;
    }
    thread_lifetime = true;
    std::thread w_gen_thread(&GameEngine::GenerateWeapon,&(*this));
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
    dw_mutex.lock();
    for(int i=0; i<drop_weapons.size(); i++) {
        Weapon* w = drop_weapons.at(i);
        if(w)
        {
        if(w->getX() == x && w->getY() == y)
        {
            dw_mutex.unlock();
            return true;
        }
        }
    }
    dw_mutex.unlock();
    return false;
}
int NullpointerDetector(std::vector<Weapon*>& p)
{
    for(int i=0; i<p.size(); i++)
    {
        if(p.at(i)==nullptr)
            return i;
    }
    return -1;
}
int FilledWithDildo(std::vector<Weapon*>& p)
{
    int sum =0;
    for(int i=0; i<p.size(); i++)
        sum+=p.at(i)!=nullptr;
    return sum;
}
void GameEngine::GenerateWeapon() {
    players_map.lock();
    int s = players.size()-1;
    players_map.unlock();
    dw_mutex.lock();
    int d = FilledWithDildo(drop_weapons);
    dw_mutex.unlock();
    while(thread_lifetime && s > d) {
        std::cout<<s<< " "<<d<<std::endl;
        srand(time(NULL));
        std::this_thread::sleep_for(std::chrono::seconds(30));
        float x=0;
        float y = 0;
        GenerateXY(x,y);
        while(GenNotGood(x, y))
            GenerateXY(x,y);
        int pos = NullpointerDetector(drop_weapons);
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
        dw_mutex.lock();
        drop_weapons[pos] = w;
        dw_mutex.unlock();
        std::stringstream ss;
        ss << "Server:" << pos << "|" << Wtype << "|" << x << "|" << y;
        server->sendData(ss.str());
        players_map.lock();
        s=players.size()-1;
        players_map.unlock();
        dw_mutex.lock();
        d = FilledWithDildo(drop_weapons);
        dw_mutex.unlock();
    }
}
std::vector<std::string> GameEngine::getState(std::string name)
{
    std::vector<std::string> result;
    players_map.lock();
    for(std::map<std::string,player*>::iterator it=players.begin(); it!=players.end(); it++)
    {
        if(it->first!=name)
        {
            result.push_back(GetMe(it->first));
        }
    }
    players_map.unlock();
    dw_mutex.lock();
    for(int i = 0; i<drop_weapons.size(); i++)
    {
        Weapon* p = drop_weapons.at(i);
        if(p!=nullptr)
        {
            std::string msg = "Server:"+std::to_string(i)+"|"+std::to_string(p->type)+"|"+std::to_string(p->getX())+"|"+std::to_string(p->getY());
            result.push_back(msg);
        }
    }
    dw_mutex.unlock();
    return result;
}
std::string GameEngine::CreatePlayer(std::string name) {
    players_map.lock();
    if (players.find(name) != players.end()) {
        players_map.unlock();
        return "Name is already used!";
    }
    float x,y = 0;
    GenerateXY(x,y);
    player* p = new player(name, x, y, 0);
    players[name]   = p;
    p_mutexes[p] = new std::mutex();
    players_map.unlock();
    std::cout<< " TO STRING "<<p->toString()<<std::endl;
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
        p->setWeapon(wp,-1);
        p_mutexes[p]->unlock();
        return name+":"+p->toString();
    }
    else
    {
    p_mutexes[p]->unlock();
    return name+":"+p->toString();
    }
}

std::string GameEngine::GetMe(std::string name) {
    p_mutexes[players[name]]->lock();
    std::string me = players[name]->toString();
    std::string ret =name+":1100" + "|"+std::to_string(players[name]->getX())+"|"+std::to_string(players[name]->getY())+"|"+std::to_string(players[name]->getRot())+me.substr(4);
    p_mutexes[players[name]]->unlock();
    return ret;
}

std::vector<std::string> GameEngine::CheckRequest(std::string name, std::string data) {
    player* actplayer = players.at(name);
    std::vector<std::string> ret;
    if(data.find("EXIT") != std::string::npos) {///EXIT
        players_map.lock();
        delete actplayer;
        players.erase(name);
        ret.push_back(name + ":" + data);
        players_map.unlock();
        return ret;
    }
    std::stringstream ss(data);
    std::string flags;
    std::string line;
    actplayer->setPoke(false);
    actplayer->setPickUp(false);
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
    if(flags.at(2) == '1') ///POKE
    {
        p_mutexes[actplayer]->lock();
        actplayer->setPoke(true);
        //std::cout<<flags.substr(0,4);
        float w_x = actplayer->getX() + cos((actplayer->getRot()-90)*3.1415/180) * weapons[actplayer->getWeapon()]->getRange();
        float w_y = actplayer->getY() + sin((actplayer->getRot()-90)*3.1415/180)* weapons[actplayer->getWeapon()]->getRange();
        bool wasNotZero = true;
        //players_map->lock();
        players_map.lock();
        for(std::pair<std::string,player*> pr : players) {
            player* p = pr.second;
            if(p != actplayer) {
                p_mutexes[p]->lock();
                if(sqrt(pow(w_x - p->getX(),2) + pow(w_y - p->getY(),2)) <= p->getHitboxRadius()) {
                    if(p->getCurrentHp() <= 0)
                        wasNotZero = false;
                    p->setCurrentHp(p->getCurrentHp() - weapons[actplayer->getWeapon()]->getPower());
                    if(p->getCurrentHp() <= 0 && wasNotZero) {
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
        players_map.unlock();
        p_mutexes[actplayer]->unlock();
    }
    if(flags.at(3) == '1') ///PICK UP A WEAPON
    {
        p_mutexes[actplayer]->lock();
        int i = 0;
        dw_mutex.lock();
        actplayer->setPickUp(false);
        bool ok = false;
        for(int i = 0; i<drop_weapons.size(); i++) {
            Weapon* x = drop_weapons.at(i);
            if(x)
            {
            float dest = sqrt(pow(x->getX() - actplayer->getX(),2) + pow(x->getY() - actplayer->getY(),2));
            if(dest <= actplayer->getHitboxRadius()) {
                actplayer->setWeapon(x->getType(),i);
                actplayer->setPickUp(true);
                drop_weapons[i] = nullptr;
                delete x;
                std::cout<<"Sikerult felvenni"<<std::endl;
                std::cout<<FilledWithDildo(drop_weapons);
                ok=true;
            }
            }
        }
        dw_mutex.unlock();
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


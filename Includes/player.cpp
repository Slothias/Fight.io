# include "player.hpp"
#include <sstream>
#include <string>
#include <iostream>
#include<fstream>
#include<thread>
//constructors************************************************
/*
player::player()
{
    maxHp =  100;
    currentHp = maxHp;
    score = 0;
    weapon = false;
}
player::player(std::string _pName)
{
    playerX=100;
    playerY=100;
    playerRotation=0;
    pName = _pName;
    maxHp =  100;
    currentHp = maxHp;
    score = 0;
    weapon = 0;

}
*/
player::player(std::string _pName, float _playerX, float _playerY, float _playerRotation)
{
    playerX = prevX = _playerX;
    playerY = prevY = _playerY;
    playerRotation = prevRot = _playerRotation;
    prevPoking=false;
    pName = _pName;
    maxHp =  100;
    currentHp = maxHp;
    score = 0;
    weapon = 0;
    hitboxRadius = 75;
    poking = pickUp = false;
}
/*player::player(float _playerX, float _playerY, float _playerRotation,
                std::string _pName, int _maxHp, int _currentHp, int _score, bool _weapon)
                : playerX(_playerX), playerY(_playerY), playerRotation(_playerRotation),
                pName(_pName), maxHp(_maxHp), currentHp(_currentHp), score(_score), weapon(_weapon) {}*/

//setters************************************************

void player::setPosition(float _playerX, float _playerY)
{
    my_mutex.lock();
    playerX = _playerX;
    playerY = _playerY;
    changed=true;
    my_mutex.unlock();
}
void player::setRotation(float _playerRotation)
{
    my_mutex.lock();
    playerRotation = _playerRotation;
    changed=true;
    my_mutex.unlock();
}
void player::setPName(std::string _pName)
{
    my_mutex.lock();
    pName = _pName;
    changed=true;
    my_mutex.unlock();
}
void player::setMaxHp(int _maxHp)
{
    my_mutex.lock();
    maxHp = _maxHp;
    changed=true;
    my_mutex.unlock();
}
void player::setRespawn(bool c)
{
    my_mutex.lock();
    respawn=c;
    my_mutex.unlock();
}
bool player::getRespawn()
{
    bool result = false;
    my_mutex.lock();
    result=respawn;
    my_mutex.unlock();
    return result;
}
void player::setCurrentHp(int _currentHp)
{
    my_mutex.lock();
    if(_currentHp <= maxHp && _currentHp > 0)
    {
        currentHp = _currentHp;
    }
    else if(_currentHp <= 0)
    {
        currentHp=0;
    }else{
        currentHp = maxHp;
    }
    changed = true;
    my_mutex.unlock();
}
void player::setScore(int _score)
{
    my_mutex.lock();
    score = _score;
    changed=true;
    my_mutex.unlock();
}
//false is the default value (no weapon)

void player::setWeapon(int _weapon)
{
    my_mutex.lock();
    weapon= _weapon;
    changed=true;
    my_mutex.unlock();
}

void player::setPoke(bool _poking) {
    my_mutex.lock();
    poking = _poking;
    my_mutex.unlock();
}

void player::setPickUp(bool _pickUp)
{
    my_mutex.lock();
    pickUp = _pickUp;
    my_mutex.unlock();
}

void player::update(std::string ) {

}

//getters************************************************

float player::getX()
{
    float result = -1;
    my_mutex.lock();
    result = playerX;
    my_mutex.unlock();
    return result;
}
float player::getY()
{
    float result = -1;
    my_mutex.lock();
    result = playerY;
    my_mutex.unlock();
    return result;
}
float player::getRot()
{
    float result = -1;
    my_mutex.lock();
    result = playerRotation;
    my_mutex.unlock();
    return result;
}
std::string player::getName()
{
    std::string result = "unknown";
    my_mutex.lock();
    result = pName;
    my_mutex.unlock();
    return result;
}
int player::getMaxHp()
{
    int result = -1;
    my_mutex.lock();
    result = maxHp;
    my_mutex.unlock();
    return result;
}
int player::getCurrentHp()
{
    int result = -1;
    my_mutex.lock();
    result = currentHp;
    my_mutex.unlock();
    return result;
}
int player::getScore()
{
    int result = -1;
    my_mutex.lock();
    result = score;
    my_mutex.unlock();
    return result;
}
int player::getWeapon()
{
    int result = -1;
    my_mutex.lock();
    result = weapon;
    my_mutex.unlock();
    return result;
}
int player::getHitboxRadius()
{
    int result = -1;
    my_mutex.lock();
    result = hitboxRadius;
    my_mutex.unlock();
    return result;
}
bool player::getPoke()
{
   my_mutex.lock();
    bool poke = poking;
    my_mutex.unlock();
    return poke;
}
bool player::getPickUp()
{
   my_mutex.lock();
    bool pick = pickUp;
    my_mutex.unlock();
    return pick;
}
std::string player::getMSG()
{
   std::stringstream s;
    s << getX() << "|" << getY() << "|" << getRot();
    return s.str();
}

std::string player::toString() {
    std::string flags;
    std::string msg;
    flags.resize(4);
    if(getX() != prevX || getY()!=prevY){
        flags[0]='1';
        msg += std::to_string(getX()) + "|"+std::to_string(getY())+"|";
        prevX = getX();
        prevY = getY();
    }
    else{
        flags[0]='0';
    }
    if(getRot() != prevRot){
        flags [1]= '1';
        msg += std::to_string(getRot()) + "|";
        prevRot = getRot();
    }
    else{
        flags [1]= '0';
    }

    if(getPoke()){
        //std::ofstream myfile("poking.txt",std::ofstream::app);
        flags [2]= '1';
        //myfile<<"POKE"<<std::endl;
        //myfile.close();
    }
    else{
        flags[2]= '0';
    }
    if(getPickUp()){
        flags[3] = '1';
    }else{
        flags[3] = '0';
    }

    flags += "|" + msg + std::to_string(getMaxHp()) + "|" + std::to_string(getCurrentHp()) + "|" + std::to_string(getScore()) + "|" + std::to_string(getWeapon());

    /*s << getX() << "|" << getY() << "|" << getRot()
    << "|" << getMaxHp() << "|" << getCurrentHp()
    << "|" << getScore() <<"|"<<getWeapon();*/
    std::cout << flags << std::endl;
    return flags;
}
void player::setChange(bool c)
{

    changed=c;
}
bool player::getChange()
{
    bool result = true;
    my_mutex.lock();
    result=changed;
    my_mutex.unlock();
    return result;
}
player::~player()
{

}

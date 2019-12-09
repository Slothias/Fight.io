# include "player.hpp"
#include <sstream>
#include <string>
#include <iostream>
#include<fstream>
#include<thread>
player::player(std::string _pName, float _playerX, float _playerY, float _playerRotation)
{
    prevX=0;
    prevY=0;
    playerX = _playerX;
    playerY = _playerY;
    playerRotation = prevRot = _playerRotation;
    prevPoking=false;
    pName = _pName;
    maxHp =  100;
    currentHp = maxHp;
    score = 0;
    level = 1;
    weapon = 0;
    weaponpos = -1;
    hitboxRadius = 75;
    poking = pickUp = false;
}

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
int player::getLevel()
{
    int ret;
    my_mutex.lock();
    ret = level;
    my_mutex.unlock();
    return ret;
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
    if(score<100)
    {
        level = 1;
        maxHp = 100;
    }
    else if(score < 250)
    {
        level = 2;
        maxHp = 150;
    }
    else if(score < 450)
    {
        level = 3;
        maxHp = 200;
    }
    else if(score < 650)
    {
        level = 4;
        maxHp = 225;
    }
    else if(score < 850)
    {
        level = 5;
        maxHp = 250;
    }
    else if(score < 1050)
    {
        level = 6;
        maxHp = 275;
    }
    else if(score < 1250)
    {
        level = 7;
        maxHp = 300;
    }
    else if(score < 1450)
    {
        level = 8;
        maxHp = 300;
    }
    else
    {
        level = 9;
        maxHp = 300;
    }
    changed=true;
    my_mutex.unlock();
}
//false is the default value (no weapon)

void player::setWeapon(int _weapon, int pos)
{
    my_mutex.lock();
    weapon= _weapon;
    weaponpos = pos;
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
        flags [2]= '1';
    }
    else{
        flags[2]= '0';
    }
    if(getPickUp()){
        flags[3] = '1';
        my_mutex.lock();
        int p = weaponpos;
        my_mutex.unlock();
        msg+= std::to_string(p)+"|";
    }else{
        flags[3] = '0';
    }

    flags += "|" + msg + std::to_string(getMaxHp()) + "|" + std::to_string(getCurrentHp()) + "|" + std::to_string(getScore()) + "|" + std::to_string(getWeapon());

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

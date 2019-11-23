# include "player.hpp"
#include <sstream>
#include <string>
#include <iostream>

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
    poking = false;
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
void player::setCurrentHp(int _currentHp)
{
    my_mutex.lock();
    if(_currentHp <= maxHp)
        {
            currentHp = _currentHp;
            changed = true;
        }
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

void player::update(std::string data) {

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

std::string player::getMSG()
{
   std::stringstream s;
    s << getX() << "|" << getY() << "|" << getRot();
    return s.str();
}

std::string player::toString() {
    std::stringstream flags;
    std::stringstream msg;

    if(getX() != prevX){
        flags << 1;
        msg << getX() << "|";
        prevX = getX();
    }
    else{
        flags << 0;
    }

    if(getY() != prevY){
        flags << 1;
        msg << getY() << "|";
        prevY = getY();
    }
    else{
        flags << 0;
    }

    if(getRot() != prevRot){
        flags << 1;
        msg << getRot() << "|";
        prevRot = getRot();
    }
    else{
        flags << 0;
    }

    if(poking != prevPoking){
        flags << 1;
        prevPoking = poking;
    }
    else{
        flags << 0;
    }
    flags << "|" << msg.str() << getMaxHp() << "|" << getCurrentHp() << "|" << getScore() << "|" << getWeapon();

    /*s << getX() << "|" << getY() << "|" << getRot()
    << "|" << getMaxHp() << "|" << getCurrentHp()
    << "|" << getScore() <<"|"<<getWeapon();*/
    //std::cout << flags.str() << std::endl << std::endl;
    return flags.str();
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

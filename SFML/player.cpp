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
    playerX = _playerX;
    playerY = _playerY;
    playerRotation = _playerRotation;
    pName = _pName;
    maxHp =  100;
    currentHp = maxHp;
    score = 0;
    weapon = 0;
    hitboxRadius = 75;
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
    my_mutex.unlock();
}
void player::setRotation(float _playerRotation)
{
    my_mutex.lock();
    playerRotation = _playerRotation;
    my_mutex.unlock();
}
void player::setPName(std::string _pName)
{
    my_mutex.lock();
    pName = _pName;
    my_mutex.unlock();
}
void player::setMaxHp(int _maxHp)
{
my_mutex.lock();
 maxHp = _maxHp;
 my_mutex.unlock();
}
void player::setCurrentHp(int _currentHp)
{
    my_mutex.lock();
    if(_currentHp <= maxHp)
        currentHp = _currentHp;
    my_mutex.unlock();
}
void player::setScore(int _score)
{
    my_mutex.lock();
    score = _score;
    my_mutex.unlock();
}
//false is the default value (no weapon)

void player::setWeapon(int _weapon)
{
    my_mutex.lock();
    weapon= _weapon;
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
    float result = -1;
    my_mutex.lock();
    result = weapon;
    my_mutex.unlock();
    return result;
}

std::string player::getMSG()
{
    //std::cout << myPlayer.getPosition().x << " " << myPlayer.getPosition().y << std::endl;
   std::stringstream s;
    s << getX() << "|" << getY() << "|" << getRot();
    return s.str();
}

std::string player::toString() {
      std::stringstream s;
      s << getX() << "|" << getY() << "|" << getRot()
        << "|" << getMaxHp() << "|" << getCurrentHp()
        << "|" << getScore() <<"|"<<getWeapon();
    return s.str();
}
player::~player()
{

}

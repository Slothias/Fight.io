# include "player.hpp"
#include <sstream>
#include <string>
#include <iostream>

//constructors************************************************

player::player()
{
    maxHp =  100;
    currentHp = maxHp;
    score = 0;
    weapon = false;
}
player::player(std::string _pName)
{
    pName = _pName;
    maxHp =  100;
    currentHp = maxHp;
    score = 0;
    weapon = false;
}
player::player(std::string _pName, float _playerX, float _playerY, float _playerRotation, sf::Texture& playerTexture)
{
    playerX = _playerX;
    playerY = _playerY;
    playerRotation = _playerRotation;
    pName = _pName;
    maxHp =  100;
    currentHp = maxHp;
    score = 0;
    weapon = false;
    myPlayer.setScale(sf::Vector2f(0.5f, 0.5f));
    myPlayer.setOrigin(playerTexture.getSize().x/2, playerTexture.getSize().y/2);
    myPlayer.setPosition(_playerX,_playerY); // todo
    myPlayer.setTexture(playerTexture);
}
//setters************************************************

void player::setPosition(float _playerX, float _playerY)
{
    playerX += _playerX;
    playerY += _playerY;
    myPlayer.setPosition(playerX,playerY);
}
void player::setRotation(float _playerRotation)
{
    playerRotation = _playerRotation;
    myPlayer.setRotation(_playerRotation);
}
void player::setPName(std::string _pName)
{
    pName = _pName;
}
void player::setMaxHp(int _maxHp)
{
    maxHp = _maxHp;
}
void player::setCurrentHp(int _currentHp)
{
    if(_currentHp <= maxHp)
        currentHp = _currentHp;
}
void player::setScore(int _score)
{
    score = _score;
}
//false is the default value (no weapon)
void player::setWeapon(bool _weapon)
{
    weapon = _weapon;
}

//getters************************************************

float player::getX()
{
    return myPlayer.getPosition().x;
}
float player::getY()
{
    return myPlayer.getPosition().y;
}
float player::getRot()
{
    return playerRotation;
}
std::string player::getName()
{
    return pName;
}
int player::getMaxHp()
{
    return maxHp;
}
int player::getCurrentHp()
{
    return currentHp;
}
int player::getScore()
{
    return score;
}
bool player::getWeapon()
{
    return weapon;
}
sf::Sprite& player::getSprite()
{
    return myPlayer;
}

std::string player::getMSG()
{
    //std::cout << myPlayer.getPosition().x << " " << myPlayer.getPosition().y << std::endl;
    std::stringstream s;
    s << "X: " << myPlayer.getPosition().x << " Y: " << myPlayer.getPosition().y << " R: " << myPlayer.getRotation();
    return s.str();
}

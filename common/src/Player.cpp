#include "Player.hpp"
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
Player::Player(std::string _pName, float _playerX, float _playerY, float _playerRotation) : pName(_pName),
                                                                                            position(_playerX, _playerY, _playerRotation),
                                                                                            prevPosition(0, 0, _playerRotation),
                                                                                            healthState(100, 100),
                                                                                            scoreState(0, 1),
                                                                                            weaponState(0, 75, -1){}


// setters************************************************

void Player::setPosition(float _playerX, float _playerY)
{
    position.x = _playerX;
    position.y =_playerY;
    changed = true;
}
void Player::setRotation(float _playerRotation)
{
    position.angle = _playerRotation;
    changed = true;
}
void Player::setPName(std::string _pName)
{
    pName = _pName;
    changed = true;
}

void Player::setMaxHp(int _maxHp)
{
    healthState.maxHp = _maxHp;
    changed = true;

}
void Player::setRespawn(bool c)
{
    respawn = c;
}
bool Player::getRespawn()
{
    return respawn;
}
void Player::setCurrentHp(int _currentHp)
{

    healthState.setCurrentHp(_currentHp);
    changed = true;
}
void Player::setScore(int _score)
{
    scoreState.setScore(_score);
    healthState.maxHp = scoreState.level*200;
    changed = true;
}
// false is the default value (no weapon)

void Player::setWeapon(int _weapon, int pos)
{   
    weaponState.weapon = _weapon;
    weaponState.weapon_pos = pos;
    changed = true;
}

void Player::setPoke(bool _poking)
{
    actionState.poke = _poking;
}

void Player::setPickUp(bool _pickUp)
{
    actionState.pickUp = _pickUp;
}

void Player::update(std::string)
{
}

// getters************************************************
Position Player::getPosition(){
    return position;
}
std::string Player::getName()
{
    return pName;;
}
HealthState Player::getHealthState(){
    return healthState;
}
ScoreState Player::getScoreState() {
    return scoreState;
}
WeaponState Player::getWeaponState() {
    return weaponState;
}
ActionState Player::getActionState() {
    return actionState;
}


std::string Player::toString()
{
  
    return "";
}
void Player::setChange(bool c)
{
    changed = c;
}
bool Player::getChange()
{
    return changed;
}
Player::~Player()
{
}

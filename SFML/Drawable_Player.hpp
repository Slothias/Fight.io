#ifndef DRAWABLE_PLAYER_H
#define DRAWABLE_PLAYER_H
#include<SFML/Graphics.hpp>
#include "../Includes/Weapon.hpp"
#include "HpBar.hpp"
#include <map>
#include<iostream>
#include<sstream>
#include "../Includes/player.hpp"
#include "Screen.hpp"

class Drawable_Player: public sf::Sprite, public player
{
public:
    Drawable_Player(std::string name,float x, float y, float a);
    virtual void setPosition(float x, float y,bool c);
    virtual void setRotation(float x, bool c);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states);
    sf::Texture getSkin();
    sf::Vector2<float> getPosition();
    virtual void setScale(float x, float y);
    virtual void setOrigin(float x, float y);

    //TODO
    std::map<std::string,Drawable_Player*> getPlayers();
    void setWeapon(int _weapon);
    void setCurrentHp(int _currentHp);
    void testPoke(bool setToIt);
    //getters
    void update(std::string data);
    Weapon getWeapon();
    std::map<std::string,Drawable_Player*> players;
    ~Drawable_Player();
    sf::CircleShape weaponHitbox;
private:
    sf::Texture skin;
    sf::Sprite me;
    Weapon myWeapon;
    HpBar* myHpBar;
    sf::CircleShape testHitbox;
    sf::Font font;
    sf::Text* myName;
};

#endif // DRAWABLE_PLAYER_H

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
#include <chrono>
#include <vector>

class Drawable_Player: public sf::Sprite, public player
{
public:
    Drawable_Player(std::string name,float x, float y, float a);
    virtual void setPosition(float x, float y,bool c);
    virtual void setRotation(float x, bool c);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states);
    virtual void outOfScreenDraw(sf::RenderTarget& target, sf::RenderStates states, double x, double y, int mapX, int mapY, unsigned int vertical);
    sf::Texture getSkin();
    sf::Vector2<float> getPosition();
    virtual void setScale(float x, float y);
    virtual void setOrigin(float x, float y);

    //TODO
    std::map<std::string,Drawable_Player*> getPlayers();

    void setWeapon(int _weapon,bool c);
    void setCurrentHp(int _currentHp);
    void testPoke(bool setToIt);
    void pickUpEvent(bool setToIt);
    void setLevel(int i);
    //getters
    void update(std::string data);
    int getMapSize();

    Weapon* getWeapon();
    std::map<std::string,Drawable_Player*> players;
    std::vector<Weapon*> getWeapons();
    bool iThinkICanPickUp();

    std::vector<Weapon*> weapons;
    int maxPlayers;
    int mapSize;
    ~Drawable_Player();
    sf::CircleShape weaponHitbox;
private:
    void readCfg();
    sf::Texture skin;
    sf::Texture deadSkin;
    sf::Sprite me;
    sf::Sprite deadMe;
    Weapon myWeapon;
    HpBar* myHpBar;
    sf::CircleShape testHitbox;
    sf::CircleShape alternativeDraw;
    sf::Font font;
    sf::Text* myName;
    std::chrono::high_resolution_clock::time_point lastPoke;
};

#endif // DRAWABLE_PLAYER_H

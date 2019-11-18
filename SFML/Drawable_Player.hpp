#ifndef DRAWABLE_PLAYER_H
#define DRAWABLE_PLAYER_H
#include<SFML/Graphics.hpp>
#include "Weapon.hpp"
#include "HpBar.hpp"
#include <mutex>
#include <map>
#include<iostream>
#include<sstream>

class Drawable_Player: public sf::Sprite
{
public:
    Drawable_Player(std::string name,float x, float y, float a);
    virtual void setPosition(float x, float y);
    virtual void setRotation(float x);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states);
    sf::Texture getSkin();
    sf::Vector2<float> getPosition();
    virtual void setScale(float x, float y);
    virtual void setOrigin(float x, float y);

    //TODO
    std::map<std::string,Drawable_Player*> getPlayers();
    void setMaxHp(int _maxHp);
    void setCurrentHp(int _currentHp);
    void setScore(int _score);
    void setWeapon(Weapon* _weapon);
    void testPoke();

    //getters
    float getX() ;
    float getY() ;
    float getRot() ;
    std::string getName() ;
    int getMaxHp();
    int getCurrentHp() ;
    int getScore() ;
    Weapon* getWeapon() ;
    std::string getMSG();
    std::string toString() ;
    void update(std::string data);
    ~Drawable_Player();
private:
    sf::Texture skin;
    sf::Sprite me;
    Weapon* myWeapon;
    HpBar* myHpBar;
    float playerX;
    float playerY;
    float playerRotation;
    std::string pName;
    int maxHp;
    int currentHp;
    int score;
    std::map<std::string,Drawable_Player*> players;
    std::mutex my_mutex;
};

#endif // DRAWABLE_PLAYER_H

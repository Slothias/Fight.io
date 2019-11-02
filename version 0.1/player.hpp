#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include<SFML/Graphics.hpp>
#include "Drawable_Player.hpp"

class player
{
private:
    float playerX;
    float playerY;
    float playerRotation;
    std::string pName;
    int maxHp;
    int currentHp;
    int score;
    std::map<std::string,Drawable_Player*> players;
public:
    std::mutex my_mutex;
    std::map<std::string,Drawable_Player*> getPlayers();
    Drawable_Player* myPlayer;
    player();
    player(std::string _pName);
    player(std::string _pName, float _playerX, float _playerY, float _playerRotation, sf::Texture& playerTexture);
    player(float _playerX, float _playerY, float _playerRotation, std::string _pName, int _maxHp, int _currentHp, int _score, bool _weapon);

    //setters
    void setPosition(float _playerX, float _playerY);
    void setRotation(float _playerRotation);
    void setPName(std::string _pName);
    void setMaxHp(int _maxHp);
    void setCurrentHp(int _currentHp);
    void setScore(int _score);
    void setWeapon(Weapon* _weapon);

    //getters
    float getX();
    float getY();
    float getRot();
    std::string getName();
    int getMaxHp();
    int getCurrentHp();
    int getScore();
    Weapon* getWeapon();
    std::string getMSG();
    std::string toString();
    sf::Sprite& getSprite();
    void update(std::string data);
};

#endif // PLAYER_H

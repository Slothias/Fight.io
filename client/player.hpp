#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <SFML/Graphics.hpp>

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
    bool weapon;
    sf::Sprite myPlayer;
public:
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
    void setWeapon(bool _weapon);

    //getters
    float getX();
    float getY();
    float getRot();
    std::string getName();
    int getMaxHp();
    int getCurrentHp();
    int getScore();
    bool getWeapon();
    sf::Sprite& getSprite();
    std::string getMSG();
    std::string toString();
};

#endif // PLAYER_H

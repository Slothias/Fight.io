#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include<SFML/Graphics.hpp>
#include "Weapon.hpp"
#include <mutex>
#include <map>
class player
{
private:
    class Drawable_Player
    {
    public:
        Drawable_Player(player* p){
            player = p;
            skin.loadFromFile("Player.png");
            me.setTexture(skin);
            player::Drawable_Player::me.setPosition(100,100);
            player::Drawable_Player::me.setRotation(p->getRot());

        }
        sf::Sprite& get();
        sf::Texture& getSkin() {return skin;};
        ~Drawable_Player();
    private:
        player* player;
        sf::Texture skin;
        sf::Sprite me;
    };
    float playerX;
    float playerY;
    float playerRotation;
    std::string pName;
    int maxHp;
    int currentHp;
    int score;
    std::map<std::string,player*> players;
public:
    std::mutex my_mutex;
    std::map<std::string,player*> getPlayers();
    Drawable_Player* myPlayer;
    Weapon* w;
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

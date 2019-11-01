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
    class Drawable_Player: public sf::Sprite
    {
    public:
        Drawable_Player(player* p){
            player = p;
            skin.loadFromFile("Player.png");
            me.setTexture(skin);
            weapon=new Weapon(10,10);
            weapon->setOrigin(-((int)skin.getSize().x/2), (weapon->skin.getSize().y/2)+50);
            setPosition();
            setRotation();

        }
        void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            target.draw(me);
            target.draw(*weapon);

        }
        void setRotation()
        {
            me.setRotation(player->getRot());
            weapon->setRotation(player->getRot());
        }
        void setPosition()
        {
            weapon->setPosition(player->getX(),player->getY());
            me.setPosition(player->getX(),player->getY());
        }
        sf::Texture getSkin()
        {
            return skin;
        }

        sf::Vector2<float> getPosition() const
        {
            return me.getPosition();
        }
        void setScale(float x, float y)
        {
            me.setScale(x,y);
            weapon->setScale(x,y);
        }
        void setOrigin(float x, float y)
        {
            me.setOrigin(x,y);
           // weapon->setOrigin(x,y);
        }
        ~Drawable_Player();
    private:
        player* player;
        sf::Texture skin;
        sf::Sprite me;
        Weapon* weapon;
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

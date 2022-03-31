#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <string>
#include <mutex>
class Player
{
protected:
    float playerX;
    float playerY;
    float playerRotation;
    bool poking;
    bool pickUp;
    float prevX, prevY, prevRot;
    bool prevPoking;
    int weaponpos;
    // sf::Vector2 hitPosition;
    std::string pName;
    int maxHp;
    int currentHp;
    int score;
    int level;
    int weapon;
    int hitboxRadius;
    bool changed;
    bool respawn;

public:
    int getLevel();
    bool getPoke();
    bool getPickUp();
    bool getRespawn();
    void setRespawn(bool c);
    std::mutex my_mutex;
    Player();
    virtual ~Player();
    Player(std::string _pName);
    Player(std::string _pName, float _playerX, float _playerY, float _playerRotation);
    Player(float _playerX, float _playerY, float _playerRotation, std::string _pName, int _maxHp, int _currentHp, int _score, bool _weapon);

    // setters
    void setChange(bool c);
    void setPosition(float _playerX, float _playerY);
    void setRotation(float _playerRotation);
    void setPName(std::string _pName);
    void setMaxHp(int _maxHp);
    void setCurrentHp(int _currentHp);
    void setScore(int _score);
    void setWeapon(int _weapon, int pos);
    void setPoke(bool _poking);
    void setPickUp(bool _pickUp);
    virtual void update(std::string data);

    // getters
    bool getChange();
    float getX();
    float getY();
    float getRot();

    std::string getName();
    int getMaxHp();
    int getCurrentHp();
    int getScore();
    int getWeapon();
    int getHitboxRadius();
    std::string getMSG();
    std::string toString();
};

#endif // PLAYER_H

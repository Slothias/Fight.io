#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <string>
#include <mutex>

struct Position
{
    float x;
    float y;
    float angle;

    Position() : x(0), y(0), angle(0) {}

    Position(float x, float y, float angle) : x(x), y(y), angle(angle) {}
};

struct ActionState
{
    bool poke;
    bool pickUp;

    ActionState() = default;
    ActionState(bool poke, bool pickUp) : poke(poke), pickUp(pickUp) {}
};

struct WeaponState
{
    int weapon;
    int hitboxRadius;
    int weapon_pos;

    WeaponState(int weapon, int hitboxRadius, int weapon_pos) : weapon(weapon), hitboxRadius(hitboxRadius), weapon_pos(weapon_pos) {}
};

struct HealthState
{
    int maxHp;
    int currentHp;

    HealthState(int maxHp, int currentHp) : maxHp(maxHp), currentHp(maxHp) {}

    void setCurrentHp(int _currentHp)
    {
        if (_currentHp <= maxHp && _currentHp > 0)
        {
            currentHp = _currentHp;
        }
        else if (_currentHp <= 0)
        {
            currentHp = 0;
        }
        else
        {
            currentHp = maxHp;
        }
    }

};

struct ScoreState
{
    int score;
    int level;

    ScoreState(int score, int level) : score(score), level(level) {}

    void setScore(int _score)
    {
        score = _score;
        level = score/150;
    }
};

class Player
{
protected:
    std::string pName;
    Position position;
    ActionState actionState;
    Position prevPosition;
    ActionState prevActionState;
    WeaponState weaponState;
    HealthState healthState;
    ScoreState scoreState;
    bool changed;
    bool respawn;

public:
    
    Position getPosition();
    ActionState getActionState();
    bool getRespawn();
    void setRespawn(bool c);
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

    std::string getName();
    HealthState getHealthState();
    ScoreState getScoreState();
    WeaponState getWeaponState();
    std::string getMSG();
    std::string toString();
};

#endif // PLAYER_H

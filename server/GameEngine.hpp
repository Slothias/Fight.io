#ifndef GAMEENGINE_HPP_INCLUDED
#define GAMEENGINE_HPP_INCLUDED

#include "../Includes/player.hpp"
#include "../Includes/Weapon.hpp"
#include <map>
#include <mutex>
#include <iostream>
#include <sstream>
#include <mutex>

class GameEngine {
private:
    static const int WP_SIZE = 6;
    std::map<std::string,player*> players;
    std::map<player*,std::mutex*> p_mutexes;
    Weapon* weapons[WP_SIZE];
    double mapSize;
protected:
    GameEngine();
public:
    /// Destr
    ~GameEngine();
    /// Create GameEngine
    static GameEngine* GetInstance();

    /// Create player with name
    std::string CreatePlayer(std::string);

    /// Get starting data
    std::string GetMe(std::string);

    /// Handle requests
    std::vector<std::string> CheckRequest(std::string, std::string);

    /// Getter mapSize
    double GetMapSize();
};

#endif // GAMEENGINE_HPP_INCLUDED

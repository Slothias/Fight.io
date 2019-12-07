#ifndef GAMEENGINE_HPP_INCLUDED
#define GAMEENGINE_HPP_INCLUDED

#include "../Includes/player.hpp"
#include "../Includes/Weapon.hpp"
#include <map>
#include <mutex>
#include <iostream>
#include <sstream>
#include <mutex>
#include "Server.hpp"

class Server;
class GameEngine {
private:
    static const int WP_SIZE = 6;
    std::map<std::string,player*> players;
    std::map<player*,std::mutex*> p_mutexes;
    std::mutex* players_map;
    Weapon* weapons[WP_SIZE];
    double mapSize;
    Server* server;
protected:
    GameEngine();
    GameEngine(Server*);
public:
    /// Destr
    ~GameEngine();
    /// Create GameEngine
    static GameEngine* GetInstance(Server*);

    /// Create player with name
    std::string CreatePlayer(std::string);
    std::string ReSpawn(std::string name);
    /// Get starting data
    std::string GetMe(std::string);

    /// Handle requests
    std::vector<std::string> CheckRequest(std::string, std::string);

    /// Getter mapSize
    double GetMapSize();
};

#endif // GAMEENGINE_HPP_INCLUDED

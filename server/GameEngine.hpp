#ifndef GAMEENGINE_HPP_INCLUDED
#define GAMEENGINE_HPP_INCLUDED

#include "../Includes/player.hpp"
#include "../Includes/Weapon.hpp"
#include <map>
#include <mutex>
#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include<utility>
#include<fstream>
#include "Server.hpp"

class Server;
class GameEngine {
private:
    static const int WP_SIZE = 6;
    std::map<std::string,player*> players;
    std::map<player*,std::mutex*> p_mutexes;
    std::vector<Weapon*> drop_weapons;
    std::mutex* dw_mutex;
    std::mutex* players_map;
    Weapon* weapons[WP_SIZE];
    double mapSize;
    int maxPlayers;
    Server* server;
    bool thread_lifetime;
protected:
    void readCfg();
    GameEngine();
    GameEngine(Server*);

    bool GenNotGood(const float&,const float&);
public:
    /// Destr
    ~GameEngine();
    /// Create GameEngine
    static GameEngine* GetInstance(Server*);

    /// Generate Weapon
    void GenerateWeapon();
    /// Create player with name
    std::string CreatePlayer(std::string);
    /// Generate pos
    void GenerateXY(float&,float&);
    /// Respawn
    std::string ReSpawn(std::string name);
    int calculateScore(player* killer, player* killed);
    /// Get starting data
    std::string GetMe(std::string);

    /// Handle requests
    std::vector<std::string> CheckRequest(std::string, std::string);
    /// Getter mapSize
    double GetMapSize();
    int GetMaxPlayers();
};

#endif // GAMEENGINE_HPP_INCLUDED

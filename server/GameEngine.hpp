#ifndef GAMEENGINE_HPP_INCLUDED
#define GAMEENGINE_HPP_INCLUDED

#include "Drawable_Player.hpp"
#include <map>
#include <mutex>


class GameEngine {
private:
    std::map<std::string,Drawable_Player*> players;
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

    /// Handle requests
    std::string CheckRequest(std::string, std::string);

    /// Getter mapSize
    double GetMapSize();
};

#endif // GAMEENGINE_HPP_INCLUDED

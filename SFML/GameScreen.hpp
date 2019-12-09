#ifndef GAMESCREEN_H
#define GAMESCREEN_H
#include "Screen.hpp"
#include "Drawable_Player.hpp"
#include<iostream>
#include "Client.hpp"
#include<fstream>
#include<vector>

class GameScreen:public Screen
{
    public:

        GameScreen(sf::RenderWindow *App,Client* c);
        void draw();
        void handle(sf::Event& event);
        std::map<std::string,Weapon*> getWeapons();
         ~GameScreen();
    private:
        void initMe(std::string msg);
        double mapSize;
        sf::Vector2<float> viewOffSet;
        static const int SLEEPVAL=1000/FRAMERATE;
        Client* c;
        Drawable_Player* me;
        bool pup;
        bool pdown;
        bool pleft;
        bool pright;
        bool pPoke;
        int tempWeaponCounter;
        float PI=3.14159;
        float ROOT2 = 1.41421356;
        float PLAYERMOVESPEED = 50;
        sf::RenderWindow *app;
        sf::Texture forBackground;
        sf::Sprite background;
        sf::RectangleShape deathOverlay;
        sf::Text youDied;
        sf::View v;
        sf::Font font;
        sf::Font deathFont;
        sf::Vector2<float> getViewOffSet();
        std::vector<sf::Text> scoreboard;
        std::map<std::string,Weapon*> weapons;
        std::string host;
        u_short port;
        unsigned int horizontal;
        unsigned int vertical;
        void GetDesktopResolution();

};

#endif // GAMESCREEN_H

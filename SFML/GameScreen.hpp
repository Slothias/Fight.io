#ifndef GAMESCREEN_H
#define GAMESCREEN_H
#include "Screen.hpp"
#include "Drawable_Player.hpp"
#include<iostream>
#include "Client.hpp"
#include<fstream>

class GameScreen:public Screen
{
    public:

        GameScreen(sf::RenderWindow *App, const char* host);
        void draw();
        void handle(sf::Event& event);
         ~GameScreen();
    private:
        Client* c;
        Drawable_Player* me;
        bool pup;
        bool pdown;
        bool pleft;
        bool pright;
        float PI=3.14159;
        float ROOT2 = 1.41421356;
        float PLAYERMOVESPEED=10;
        sf::RenderWindow *app;
        sf::Texture forBackground;
        sf::Sprite background;
        sf::View v;
        sf::Font font;
        std::string host;
        u_short port;
        unsigned int horizontal;
        unsigned int vertical;
        void GetDesktopResolution();

};

#endif // GAMESCREEN_H

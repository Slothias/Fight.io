#ifndef GAMESCREEN_H
#define GAMESCREEN_H
#include "Screen.hpp"
#include "player.hpp"
#include<iostream>
#include "Client.hpp"

class GameScreen:public Screen
{
    public:

        GameScreen(sf::RenderWindow *App);
        void draw();
        sf::Music& getMusic();
        void handle(sf::Event event);
         ~GameScreen();
    private:
        Client* c;
        bool pup;
        bool pdown;
        bool pleft;
        bool pright;
        static const int FRAMERATE=120;
        player* me;
        sf::Music music;
        float PI=3.14159;
        float ROOT2 = 1.41421356;
        float PLAYERMOVESPEED=10;
        sf::RenderWindow *app;
        sf::Texture forBackground;
        sf::Sprite background;
        sf::View v;
        sf::Font font;

};

#endif // GAMESCREEN_H

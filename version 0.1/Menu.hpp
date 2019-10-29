#ifndef MENU_H
#define MENU_H

#include<iostream>
#include "Screen.hpp"
class Menu: public Screen
{
    public:
        Menu(sf::RenderWindow *App);
        void draw();
        sf::Music& getMusic();
        void handle(sf::Event event);
        ~Menu();
    private:
        sf::RenderWindow * app;
        sf::Font font;
        std::vector<sf::Text*> texts;
        void loadTexts();
        void drawTexts();
        void textSelect();
        sf::Music music;
        sf::Texture forCursor;
        sf::Texture forBackground;
        sf::Sprite background;
         sf::Font getFont();
        sf::Sprite cursor;
};

#endif // MENU_H

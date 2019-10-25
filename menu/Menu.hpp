#ifndef MENU_H
#define MENU_H

#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
class Menu: public sf::RenderWindow
{
    public:
        Menu(sf::VideoMode vm, std::string title);
        void loop();
        void setFont(std::string path);
        void loadCursosIMG(std::string path);
        void loadBackground(std::string path);
        void setMusic(std::string path);
        virtual ~Menu();
    private:
        sf::Image icon;
        sf::Music music;
        sf::Texture forCursor;
        sf::Texture forBackground;
        sf::Font font;
        sf::Sprite cursor;
        sf::Sprite background;
        sf::View view;
        std::vector<sf::Text*> texts;
        void loadTexts();
        void drawTexts();
        void textSelect();
};

#endif // MENU_H

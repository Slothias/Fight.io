#ifndef WINDOW_HPP
#define WINDOW_HPP
#include "Menu.hpp"
#include "GameScreen.hpp"
#include "ConnectScreen.hpp"
class Window:public sf::RenderWindow
{
private:
    public:
        sf::View view;
        Window(sf::VideoMode vm, std::string title);
        void loop();
        State getState();
        void setState(State state);
        ~Window();
        Menu* menu;
        GameScreen* gameScreen;
        ConnectScreen* connectScreen;
        State state;
        sf::Image icon;
        //sf::Music music;
        //sf::Music game;


};

#endif // WINDOW_H

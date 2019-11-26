#ifndef WINDOW_H
#define WINDOW_H
#include "Menu.hpp"
#include "GameScreen.hpp"
#include "ConnectScreen.hpp"
class Window:public sf::RenderWindow
{
private:
   // static const int SLEEPVAL=1000/FRAMERATE;
    public:
       // sf::Event event;
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


};

#endif // WINDOW_H

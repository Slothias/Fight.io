#ifndef WINDOW_H
#define WINDOW_H
#include "Menu.hpp"
#include "GameScreen.hpp"
class Window:public sf::RenderWindow
{
private:
   // static const int SLEEPVAL=1000/FRAMERATE;
    public:
        sf::View view;
        Window(sf::VideoMode vm, std::string title);
        void loop();
        State getState();
        void setState(State state);
        ~Window();
        std::vector<Screen*> screens;
        State state;
        sf::Image icon;


};

#endif // WINDOW_H

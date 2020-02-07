#include "Window.hpp"
#include <thread>
#include<chrono>
#include<unistd.h>
#include<fstream>
Window::Window(sf::VideoMode vm, std::string title):RenderWindow(vm,title)
{
    state=State::main_menu;
    menu = new Menu(this);
    connectScreen = nullptr;
    gameScreen = nullptr;
    icon.loadFromFile("Player.png");
    setIcon(150,100,icon.getPixelsPtr());
    setVerticalSyncEnabled(false);
    /*music.openFromFile("we_are_the_resistors.wav");
    music.setVolume(20);
    game.openFromFile("menu_select.wav");
    game.setVolume(20);*/
    setFramerateLimit(120);
}
void Window::loop()
{
    setMouseCursorVisible(false);
   // music.play();
    while(state==State::main_menu && isOpen())//connect screen
    {
        menu->draw();
        display();
        sf::Event event;
        if(pollEvent(event)){
            menu->handle(event);
        }
        if(menu->change_me())
        {
            state=State::connecting;
            connectScreen = new ConnectScreen(this);
        }
    }

    setMouseCursorVisible(false);
    while(state==State::connecting && isOpen())//connect screen
    {

        clear(sf::Color::Black);
        connectScreen->draw();
        display();
        sf::Event event;
        if(pollEvent(event)){
            connectScreen->handle(event);
        }
        if(connectScreen->change_me())
        {
            state=State::play;
           // music.stop();
            gameScreen = new GameScreen(this,connectScreen->getClient());
        }

    }
    setMouseCursorVisible(true);
    //game.play();
    while(state==State::play && isOpen())//game screen
    {
        gameScreen->draw();
        sf::Event event;
        pollEvent(event);
        gameScreen->handle(event);
        display();

    }
   // game.stop();
}
void Window::setState(State s)
{
    state=s;
}
State Window::getState()
{
    return state;
}
Window::~Window()
{
}

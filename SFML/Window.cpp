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
    //screens.push_back(new GameScreen(this));
    icon.loadFromFile("Player.png");
    setIcon(150,100,icon.getPixelsPtr());
    setVerticalSyncEnabled(false);
}
void Window::loop()
{
    setMouseCursorVisible(false);
    while(state==State::main_menu)//connect screen
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

    setMouseCursorVisible(true);
    while(state==State::connecting)//connect screen
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
            gameScreen = new GameScreen(this, connectScreen->getIP());
        }

    }

    setMouseCursorVisible(true);
    while(isOpen())//game screen
    {   //auto t1 = std::chrono::high_resolution_clock::now();

      /*  if(screens[state]->getMusic().getStatus()!=sf::SoundSource::Status::Playing)
        {
            if(state==State::play && screens[state-1]->getMusic().getStatus()==sf::SoundSource::Status::Playing)
                screens[state-1]->getMusic().stop();
            screens[state]->getMusic().play();

        }*/
        /////////////////////////////////// valami nem jó vele
        gameScreen->draw();


        sf::Event event;
        pollEvent(event);
        gameScreen->handle(event);
      /* std::thread t([&]()
        {
            gameScreen->handle(event);
        });
        t.detach();*/
        display();
        //auto t2 = std::chrono::high_resolution_clock::now();
      //std::this_thread::sleep_for(std::chrono::nanoseconds(12));/////////////////////////////////////////// baj van vele
        //auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        //std::cout << duration << std::endl;
    }
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

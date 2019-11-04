#include "Window.hpp"
#include <thread>
Window::Window(sf::VideoMode vm, std::string title):RenderWindow(vm,title)
{
    state=State::main_menu;
    screens.push_back(new Menu(this));
    screens.push_back(new ConnectScreen(this));
    screens.push_back(new GameScreen(this));
    icon.loadFromFile("Player.png");
    setIcon(150,100,icon.getPixelsPtr());
  //  setVerticalSyncEnabled(true);
}
void Window::loop()
{

    while(isOpen())
    {
      /*  if(screens[state]->getMusic().getStatus()!=sf::SoundSource::Status::Playing)
        {
            if(state==State::play && screens[state-1]->getMusic().getStatus()==sf::SoundSource::Status::Playing)
                screens[state-1]->getMusic().stop();
            screens[state]->getMusic().play();

        }*/
         clear(sf::Color::Black);
        screens[state]->draw();
        display();
        sf::Event event;
        pollEvent(event);
        screens[state]->handle(event);
        if(screens[state]->change_me())
        {
            state=State::play;
        }
        if(state==State::play)
            setMouseCursorVisible(true);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000/120));
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
    for(Screen* s:screens)
        delete s;
    screens.clear();
}

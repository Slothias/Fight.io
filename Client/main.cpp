#include "Window.hpp"

int main()
{
    Window w(sf::VideoMode(800,600),"Fight.io");
    w.loop();
    return 0;
}

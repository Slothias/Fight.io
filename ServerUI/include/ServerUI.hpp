#ifndef SERVERUI
#define SERVERUI

#include "button/ConnectionButton.hpp"

class ServerUI
{
private:
    simplgui::Theme theme;
    sf::Font font;
    sf::Text message;
    
    sf::RenderWindow renderWindow;
    std::unique_ptr<ConnectionButton> connectButton;


    void initializeComponenets();

    void initializeWindow();
    void initializeTexts();

    static void setWidgetToCenter(sf::Transformable& transformable, sf::Window& window);
public:
    ServerUI(unsigned int width, unsigned int height,std::string title);
    void showWindow();
    ~ServerUI();
};

#endif /* SERVERUI */

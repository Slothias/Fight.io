#include "ServerUI.hpp"
#include<iostream>
ServerUI::ServerUI(unsigned int width, unsigned int height, std::string title) : renderWindow(sf::VideoMode(width, height), title), 
theme(simplgui::Theme::defaultTheme()), connectButton(std::make_unique<ConnectionButton>(theme))
{
    initializeComponenets();
}

void ServerUI::initializeWindow()
{
    renderWindow.setFramerateLimit(30);
}

void ServerUI::initializeTexts(){
    font.loadFromFile(theme.getProperty<std::string>("font"));

    message.setFont(font);
    message.setFillColor(sf::Color::Red);
    message.setString("kutyafasz");
    setWidgetToCenter(message,renderWindow);
    message.move(sf::Vector2f(message.getGlobalBounds().width/-2.0f,message.getGlobalBounds().height/-2.0f));
    
    
}

void ServerUI::setWidgetToCenter(sf::Transformable& transformable, sf::Window& window){
    sf::Vector2 windowSize = window.getSize();
    float divider = -2.0f;

    sf::Vector2f newOrigin  = sf::Vector2f(windowSize.x/divider, windowSize.y/divider);

    transformable.setOrigin(newOrigin);
    std::cout<<"newOrigin: "<<newOrigin.x<<" : "<<newOrigin.y<<std::endl;
}

void ServerUI::initializeComponenets() {
    initializeWindow();
    initializeTexts();
}

void ServerUI::showWindow(){
     while (renderWindow.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (renderWindow.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                renderWindow.close();

         //     connB->processEvent(simplgui::Event(event, window));
        }


        renderWindow.clear();
        renderWindow.draw(message);
        renderWindow.display();
    }
}

ServerUI::~ServerUI()
{
}
#include "ConnectScreen.hpp"
#include<thread>
#include<iostream>
ConnectScreen::ConnectScreen(sf::RenderWindow *a):Screen()
{
    app=a;

    const int centerX = (app->getSize().x)/2;
    const int centerY = (app->getSize().y)/2;
    const int textboxX = 300;
    const int textboxY = 48;


    myTheme=simplgui::Theme::defaultTheme();
    auto resGetter = simplgui::FileResourcesGetter::create();

    button = simplgui::Button::create(resGetter);
    button->setLabel(U"Connect");

    const int buttonX = (button->getLabel()).size()*18;
    const int buttonY = textboxY;

    button->setPosition(sf::Vector2f(centerX-((textboxX+buttonX)/2)+textboxX, centerY-(buttonY/2)));
    button->setSize(sf::Vector2f(buttonX,buttonY));
    button->setTheme(myTheme);
    button->onClicked.bind([&](simplgui::Button::Ptr button)
    {
        getIP();
        change=true;
    });

    textbox=simplgui::TextBox::create(resGetter);
    textbox->setPosition(sf::Vector2f(centerX-((textboxX+buttonX)/2),centerY-(textboxY/2)));
    textbox->setText(U"127.0.0.1");
    textbox->setSize(sf::Vector2f(textboxX, textboxY));
    textbox->setTheme(myTheme);

    forBackground.loadFromFile("Menu.png");
    background.setTexture(forBackground);
}

const char* ConnectScreen::getIP()
{
    std::u32string ip = textbox->getText();
    char* chr = new char[ip.length()];
    for(int i=0; i<ip.length(); i++)
    {
        chr[i] = ip[i];
    }
    const char* result = chr;
    return result;
}

void ConnectScreen::draw()
{
    app->draw(background);
    auto dt = clock.restart();
    textbox->update(dt);
    button->update(dt);
    app->draw(*textbox);
    app->draw(*button);
}
void ConnectScreen::handle(sf::Event& event)
{
    if(event.type == sf::Event::Closed)
    {
    app->close();
    }
    else
    {
        if(event.type==sf::Event::TextEntered || event.type == sf::Event::MouseButtonPressed)
        {
        textbox->processEvent(simplgui::Event(event, *app));
        //std::this_thread::sleep_for(std::chrono::milliseconds(80));
        }
        button->processEvent(simplgui::Event(event, *app));
    }
}

ConnectScreen::~ConnectScreen()
{

}

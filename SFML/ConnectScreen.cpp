#include "ConnectScreen.hpp"
#include<thread>
ConnectScreen::ConnectScreen(sf::RenderWindow *a):Screen()
{
    app=a;
    myTheme=simplgui::Theme::defaultTheme();
    auto resGetter = simplgui::FileResourcesGetter::create();
    textbox=simplgui::TextBox::create(resGetter);
    textbox->setPosition(sf::Vector2f(100, 100));
    textbox->setText(U"Some text");
    textbox->setSize(sf::Vector2f(300, simplgui::AUTO_SIZE));
    textbox->setTheme(myTheme);
    button = simplgui::Button::create(resGetter);
    button->setPosition(sf::Vector2f(408, 100));
    button->setMinSize(sf::Vector2f(48, simplgui::NO_MIN_SIZE));
    button->setTheme(myTheme);
    button->setLabel(U"Clear text");
    button->onClicked.bind([&](simplgui::Button::Ptr button)
    {
        getIP();
        change=true;
    });
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

#include "ConnectScreen.hpp"
#include<thread>
#include <ctype.h>
#include<sstream>

ConnectScreen::ConnectScreen(sf::RenderWindow *a):Screen()
{
    app=a;

    forCursor.loadFromFile("Kard.png");
    cursor.setTexture(forCursor);

    const int centerX = (app->getSize().x)/2;
    const int centerY = (app->getSize().y)/2;
    const int textboxX = 300;
    const int textboxY = 48;

    font.loadFromFile("ARCADECLASSIC.ttf");
    resultText = new sf::Text("",font,30);

    resultText->setPosition(app->getView().getCenter().x/2 - resultText->getLocalBounds().width/2, app->getSize().y-4*resultText->getLocalBounds().height);
    resultText->setColor(sf::Color::Red);

    testClient =new Client();

    myTheme=simplgui::Theme::defaultTheme();
    auto resGetter = simplgui::FileResourcesGetter::create();

    button = simplgui::Button::create(resGetter);
    button->setLabel(U"Connect");

    const int buttonX = (button->getLabel()).size()*18;
    const int buttonY = textboxY;
    isconnecting=false;

    button->setPosition(sf::Vector2f(centerX-((textboxX+buttonX)/2)+textboxX, centerY-(buttonY/2)));
    button->setSize(sf::Vector2f(buttonX,buttonY));
    button->setTheme(myTheme);
    button->onClicked.bind([&](simplgui::Button::Ptr button)
    {
        std::thread t([this]()
                      {
        resultText->setString("");
        std::string res = getIP();
        my_mutex.lock();
        isconnecting=true;
        my_mutex.unlock();
        const std::string result =  testClient->tryToConnect(res.c_str(),10043,getName());
        resultText->setString(result);
        resultText->setPosition(app->getView().getCenter().x/2 - resultText->getLocalBounds().width/2, app->getSize().y-4*resultText->getLocalBounds().height);
        my_mutex.lock();
        change = result == "OK";
        isconnecting=false;
        my_mutex.unlock();
                      });
        t.detach();

    });

    textbox=simplgui::TextBox::create(resGetter);
    textbox->setPosition(sf::Vector2f(centerX-((textboxX+buttonX)/2),centerY-(textboxY/2)));
    textbox->setText(U"127.0.0.1");
    textbox->setSize(sf::Vector2f(textboxX, textboxY));
    textbox->setTheme(myTheme);

    name = simplgui::TextBox::create(resGetter);
    name->setPosition(sf::Vector2f(centerX-(textboxX+buttonX)/2,centerY-2*textboxY));
    name->setText(U"Name");
    name->setSize(sf::Vector2f(textboxX,textboxY));
    name->setTheme(myTheme);

    forBackground.loadFromFile("Menu.png");
    background.setTexture(forBackground);
}
std::string ConnectScreen::getName()
{
    std::string result;
    std::u32string n = name->getText();
    int maxLength = 10;
    for(char c : n){
        if(maxLength>0)
            result.push_back(c);
        maxLength--;
    }

    return result;

}
Client* ConnectScreen::getClient()
{
    return testClient;
}
bool isDigit(char c)
{
    return c>=48 && c<=57;
}
std::string ConnectScreen::getIP()
{
    std::u32string ip = textbox->getText();
    std::string str="";;
    for(int i=0; i<ip.length(); i++)
    {
        if(isDigit(ip[i])|| ip[i]=='.')
            {
                str.push_back((char)ip[i]);
            }
    }
    return str;
}

void ConnectScreen::draw()
{
    app->draw(background);
    auto dt = clock.restart();
    textbox->update(dt);
    name->update(dt);
    button->update(dt);
    app->draw(*textbox);
    app->draw(*button);
    app->draw(*name);
    if(resultText->getString().toAnsiString()!="OK");
        app->draw(*resultText);
    app->draw(cursor);
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
        bool g =false;
        my_mutex.lock();
        g=isconnecting;
        my_mutex.unlock();
        if(!g)
            name->processEvent(simplgui::Event(event,*app));
        }
        button->processEvent(simplgui::Event(event, *app));
        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*app)));
    }
}

ConnectScreen::~ConnectScreen()
{

}

#include "Menu.hpp"

Menu::Menu(sf::VideoMode vm,std::string title):sf::RenderWindow(vm,title,sf::Style::Close)
{
    icon.loadFromFile("Player.png");
    setIcon(150,100,icon.getPixelsPtr());
    setFont("ARCADECLASSIC.ttf");
    loadCursosIMG("Kard.png");
    loadBackground("Menu.png");
    setMouseCursorVisible(false);
    view=getView();
    loadTexts();
    music.setLoop(true);
    setMusic("we_are_the_resistors.wav");
    music.setVolume(50);
}

void Menu::loadTexts()
{
    texts.push_back(new sf::Text("Play",font,30));
    texts.push_back(new sf::Text("Instruction",font,30));
    texts.push_back(new sf::Text("Quit",font,30));

    for(int i=0; i<texts.size(); i++)
    {
        float currentx = view.getCenter().x-texts[i]->getGlobalBounds().width/2;
        float currenty = getSize().y-(texts.size()-i)*getSize().y/6;
        texts[i]->setColor(sf::Color::Black);
        texts[i]->setPosition(currentx,currenty);

    }
}
void Menu::setMusic(std::string path)
{
    music.openFromFile(path);
}
void Menu::setFont(std::string path)
{
    font.loadFromFile(path);
}
void Menu::loadCursosIMG(std::string path)
{

    forCursor.loadFromFile(path);
    cursor.setTexture(forCursor);
}
void Menu::loadBackground(std::string path)
{
    forBackground.loadFromFile(path);
    background.setTexture(forBackground);
}
void Menu::drawTexts()
{
    for(sf::Text* t: texts)
        draw(*t);

}

void Menu::textSelect()
{
    float posx = sf::Mouse::getPosition(*this).x;
    float posy = sf::Mouse::getPosition(*this).y;

    for(sf::Text* t:texts)
    {
        float min_x = t->getPosition().x;
        float max_x = min_x+t->getLocalBounds().width;

        float min_y =t->getPosition().y;
        float max_y = min_y+2*t->getLocalBounds().height;

        if(posx>=min_x  && posx<=max_x && posy>=min_y && posy<=max_y)
           {
            if(t->getString()=="Quit")
            {
                t->setColor(sf::Color::Red);
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    close();
            }
            else
                t->setColor(sf::Color::Green);
           }
        else
            t->setColor(sf::Color::Black);
    }
}
void Menu::loop()
{
    setVerticalSyncEnabled(true);
    music.play();
    while(isOpen())
    {
        sf::Event event;
        while(pollEvent(event))
        {

            if(event.type == sf::Event::Closed || event.KeyPressed==sf::Keyboard::Escape)
            {
                close();
            }
            textSelect();
        }
        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*this)));
        clear(sf::Color::Green);
        setView(view);
        draw(background);
        drawTexts();
        draw(cursor);
        display();
    }
}
Menu::~Menu()
{
    for(sf::Text* t:texts)
        delete t;

}

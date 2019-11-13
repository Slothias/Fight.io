#include "Menu.hpp"

Menu::Menu(sf::RenderWindow* a):Screen()
{
    app=a;
    font.loadFromFile("ARCADECLASSIC.ttf");
    loadTexts();
    forCursor.loadFromFile("Kard.png");
    cursor.setTexture(forCursor);
    forBackground.loadFromFile("Menu.png");
    forBackground.setRepeated(true);
    background.setTexture(forBackground);
    app->setMouseCursorVisible(false);
 /*   music.setLoop(true);
    music.openFromFile("we_are_the_resistors.wav");
    music.setVolume(20);
*/
}

void Menu::loadTexts()
{
    texts.push_back(new sf::Text("Play",font,30));
    texts.push_back(new sf::Text("Instruction",font,30));
    texts.push_back(new sf::Text("Quit",font,30));

    for(int i=0; i<texts.size(); i++)
    {
        float currentx = app->getView().getCenter().x-texts[i]->getGlobalBounds().width/2;
        float currenty = app->getSize().y-(texts.size()-i)*app->getSize().y/6;
        texts[i]->setColor(sf::Color::Black);
        texts[i]->setPosition(currentx,currenty);

    }
}
void Menu::drawTexts()
{
    for(sf::Text* t: texts)
        app->draw(*t);
}
void Menu::textSelect()
{
    float posx = sf::Mouse::getPosition(*app).x;
    float posy = sf::Mouse::getPosition(*app).y;
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
                if(t->getColor()!=sf::Color::Red)
                {
                    t->setColor(sf::Color::Red);
                }
                else if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        app->close();
            }
            else if (t->getColor()!=sf::Color::Green)
            {
             t->setColor(sf::Color::Green);

           }
           if(t->getString()=="Play" && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                   change=true;
           }
        else
            t->setColor(sf::Color::Black);
    }
}
void Menu::draw()
{
app->draw(background);
drawTexts();
app->draw(cursor);


}
void Menu::handle(sf::Event& event)
{
    if(event.type == sf::Event::Closed || event.KeyPressed==sf::Keyboard::Escape)
    {
        app->close();
    }
    else{
    textSelect();
    cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*app)));
    }
}
Menu::~Menu()
{
    for(sf::Text* t:texts)
        delete t;

}

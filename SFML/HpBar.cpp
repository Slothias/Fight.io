#include "HpBar.hpp"
#include <sstream>

HpBar::HpBar(int m,float sx, float sy):sf::Sprite()
{
    currentHp=maxHp=m;
    skin.loadFromFile("hpBarBackground.png");
    border.setTexture(skin);
    hpBar.setSize(sf::Vector2f(100,23));
    hpBar.setFillColor(sf::Color::Green);
    font.loadFromFile("ARCADECLASSIC.ttf");
    level = new sf::Text("1",font,50);
    level->setFillColor(sf::Color::Black);
}
void HpBar::setPosition(float x, float y)
{
    border.setPosition(x,y);
    hpBar.setPosition(x+3,y+14);
    level->setPosition(x+skin.getSize().x-38,y-10);
}
/*void HpBar::setRotation(float angle)
{
    border.setRotation(angle);
    hp.setRotation(angle);
}*/

void HpBar::setColor(sf::Color color)
{
    hpBar.setFillColor(color);
}
void HpBar::setMaxHp(int m)
{
    maxHp =m;
}
void HpBar::draw(sf::RenderTarget& target, sf::RenderStates rs)
{
    target.draw(border);
    target.draw(hpBar);
    target.draw(*level);
}

void HpBar::setMyLevel(int l)
{
    std::stringstream ss;
    ss<<l;
    std::string s;
    ss>>s;
    const std::string lev(s);
    level->setString(lev);
}

void HpBar::setCurrentHp(int c)
{
    if(c>0)
    {
    currentHp =c;
    hpBar.setSize(sf::Vector2f((float)(100*currentHp/maxHp),hpBar.getSize().y));
    }
}

HpBar::~HpBar()
{
    //dtor
}

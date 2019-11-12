#include "HpBar.hpp"

HpBar::HpBar(int m,float sx, float sy):sf::Sprite()
{
    currentHp=maxHp=m;
    skin.loadFromFile("hpBarBackground.png")
    border.setTexture(skin);
    hp.setSize(sf::Vector2f(sx,sy));
    hp.setFillColor(sf::Color::Green);
}
void HpBar::setPosition(float x, float y)
{
    hp.setPosition(x,y);
}
void HpBar::setRotation(float angle)
{
    border.setRotation(angle);
    hp.setRotation(angle);
}

void HpBar::setColor(sf::Color color)
{
    hp.setFillColor(color);
}
void HpBar::setMaxHp(int m)
{
    maxHp =m;
}
void HpBar::draw(sf::RenderTarget& target, sf::RenderStates rs)
{
    target.draw(border);
    target.draw(hp);
}
void HpBar::setCurrentHp(int c)
{
    currentHp =c;
}

HpBar::~HpBar()
{
    //dtor
}

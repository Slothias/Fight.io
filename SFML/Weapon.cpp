#include "Weapon.hpp"

Weapon::Weapon():sf::Sprite()
{
}

Weapon::Weapon(int type):sf::Sprite()
{
    switch(type)
    {
        case 0: skin.loadFromFile("w0.png");
            break;
        case 1: skin.loadFromFile("w1.png");
            break;
        case 2: skin.loadFromFile("w2.png");
            break;
        case 3: skin.loadFromFile("w3.png");
            break;
        case 4: skin.loadFromFile("w4.png");
            break;
        case 5: skin.loadFromFile("w5.png");
            break;
    }
    skin.setSmooth(true);
    setTexture(skin);
    setScale(200.0f,200.0f);
}

void Weapon::setPower(float p)
{
    power=p;
}
void Weapon::setRange(float r)
{
    range=r;
}
float Weapon::getPower() const
{
    return power;
}
float Weapon::getRange() const
{
    return range;
}
Weapon::~Weapon()
{

}

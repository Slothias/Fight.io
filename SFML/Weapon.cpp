#include "Weapon.hpp"

Weapon::Weapon():sf::Sprite()
{
}

Weapon::Weapon(int type):sf::Sprite()
{
    this->type=type;
    switch(type)
    {
        case 0: skin.loadFromFile("w0.png");
                useRotation = 90;
            break;
        case 1: skin.loadFromFile("w1.png");
                useRotation = 80;
            break;
        case 2: skin.loadFromFile("w2.png");
                useRotation = 70;
            break;
        case 3: skin.loadFromFile("w3.png");
                useRotation = 50;
            break;
        case 4: skin.loadFromFile("w4.png");
                useRotation = 40;
            break;
        case 5: skin.loadFromFile("w5.png");
                useRotation = 30;
            break;
    }
    skin.setSmooth(true);
    setTexture(skin);
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

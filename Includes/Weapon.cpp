#include "Weapon.hpp"

Weapon::Weapon():sf::Sprite()
{
}

void Weapon::loadWeapon(int i)
{
    type=i;
    switch(type)
    {
        case 0: skin.loadFromFile("w0.png");
                useRotation = 45;
                range = 90;
                cooldown = 500;
                power = 15;
            break;
        case 1: skin.loadFromFile("w1.png");
                useRotation = 33;
                range = 128;
                cooldown = 750;
                power = 20;
            break;
        case 2: skin.loadFromFile("w2.png");
                useRotation = 17;
                range = 223;
                cooldown = 750;
                power = 25;
            break;
        case 3: skin.loadFromFile("w3.png");
                useRotation = 20;
                range = 218;
                cooldown = 750;
                power = 50;
            break;
        case 4: skin.loadFromFile("w4.png");
                useRotation = 17;
                range = 222;
                cooldown = 1000;
                power = 75;
            break;
        case 5: skin.loadFromFile("w5.png");
                useRotation = 17;
                range = 223;
                cooldown = 1250;
                power = 100;
            break;
    }
    skin.setSmooth(true);
    setTexture(skin);
}
Weapon::Weapon(int type):sf::Sprite()
{
    loadWeapon(type);
}

int Weapon::getUseRotation()
{
    return useRotation;
}

void Weapon::setPower(float p)
{
    power=p;
}
void Weapon::setRange(float r)
{
    range=r;
}
void Weapon::setXY(float _x, float _y)
{
    x = _x;
    y = _y;
}
float Weapon::getX()
{
    return x;
}
float Weapon::getY()
{
    return y;
}
int Weapon::getType()
{
    return type;
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

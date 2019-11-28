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
                useRotation = 55;
                range = 87;
            break;
        case 1: skin.loadFromFile("w1.png");
                useRotation = 36;
                range = 125;
            break;
        case 2: skin.loadFromFile("w2.png");
                useRotation = 17;
                range = 220;
            break;
        case 3: skin.loadFromFile("w3.png");
                useRotation = 20;
                range = 218;
            break;
        case 4: skin.loadFromFile("w4.png");
                useRotation = 17;
                range = 230;
            break;
        case 5: skin.loadFromFile("w5.png");
                useRotation = 17;
                range = 220;
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

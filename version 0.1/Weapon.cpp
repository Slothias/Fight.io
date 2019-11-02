#include "Weapon.hpp"

Weapon::Weapon():sf::Sprite()
{
    skin.loadFromFile("Spearr.png");
    setTexture(skin);
    setScale(0.5f,0.5f);
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

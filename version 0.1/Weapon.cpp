#include "Weapon.hpp"

Weapon::Weapon(float p, float r):sf::Sprite()
{
    power=p;
    range=r;
    skin.loadFromFile("Spearr.png");
    setTexture(skin);
    setScale(0.5f,0.5f);
}

Weapon::~Weapon()
{

}

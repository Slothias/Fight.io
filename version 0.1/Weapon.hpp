#ifndef WEAPON_H
#define WEAPON_H
#include<SFML/Graphics.hpp>

class Weapon: public sf::Sprite
{
    public:
        Weapon(float p, float r);
        virtual ~Weapon();
        sf::Texture skin;
        float range;
        float power;
};

#endif // WEAPON_H

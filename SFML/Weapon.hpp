#ifndef WEAPON_H
#define WEAPON_H
#include<SFML/Graphics.hpp>
class Weapon: public sf::Sprite
{
    public:
        int type;
        Weapon();
        Weapon(int type);
        virtual ~Weapon();
        sf::Texture skin;
        void setPower(float p);
        void setRange(float r);
        float getPower() const;
        float getRange() const;
        float range;
        float power;
};

    #endif // WEAPON_H

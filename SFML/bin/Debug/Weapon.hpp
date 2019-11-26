#ifndef WEAPON_H
#define WEAPON_H
#include<SFML/Graphics.hpp>
class Weapon: public sf::Sprite
{
    public:
        int type;
        int useRotation;
        Weapon();
        Weapon(int tpe);
        virtual ~Weapon();
        sf::Texture skin;
        void setPower(float p);
        void setRange(float r);
        int getUseRotation();
        float getPower() const;
        float getRange() const;
        float range;
        float power;
};

    #endif // WEAPON_H

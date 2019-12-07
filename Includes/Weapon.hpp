#ifndef WEAPON_H
#define WEAPON_H
#include<SFML/Graphics.hpp>
class Weapon: public sf::Sprite
{
    public:
        int type;
        int useRotation;
        int cooldown;
        Weapon();
        Weapon(int type);
        virtual ~Weapon();
        sf::Texture skin;
        void setPower(float p);
        void setRange(float r);
        int getUseRotation();
        float getPower() const;
        float getRange() const;
        float range;
        float power;
        void loadWeapon(int i);
};

    #endif // WEAPON_H

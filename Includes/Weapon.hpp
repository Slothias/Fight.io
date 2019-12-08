#ifndef WEAPON_H
#define WEAPON_H
#include<SFML/Graphics.hpp>
class Weapon: public sf::Sprite
{
    public:
        float x,y;
        int type;
        int useRotation;
        int cooldown;
        Weapon();
        Weapon(int type);
        virtual ~Weapon();
        sf::Texture skin;
        void setPower(float p);
        void setRange(float r);
        void setXY(float _x, float _y);
        float getX();
        float getY();
        int getType();
        int getUseRotation();
        float getPower() const;
        float getRange() const;
        float range;
        float power;
        void loadWeapon(int i);
};

    #endif // WEAPON_H

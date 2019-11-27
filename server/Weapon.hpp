#ifndef WEAPON_H
#define WEAPON_H
#include<SFML/Graphics.hpp>
class Weapon: public sf::Sprite
{
    //absolutePositionTester.setPosition(x+(cos((playerRotation-90)* 3.1415 / 180.0)*myWeapon->range),y+(sin((playerRotation-90)* 3.1415 / 180.0)*myWeapon->range));
    public:
        int type;
        int useRotation;
        Weapon();
        Weapon(int tpe);
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

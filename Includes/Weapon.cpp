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

    alternativeDraw.setFillColor(sf::Color(150,150,150,150));
    alternativeDraw.setRadius(10);
    alternativeDraw.setPosition(-10,-10);
    alternativeDraw.setOutlineColor(sf::Color(0,0,0,150));
    alternativeDraw.setOutlineThickness(3);

    weaponPickupPoint.setFillColor(sf::Color(150,150,150,150));
    weaponPickupPoint.setRadius(5);
    weaponPickupPoint.setPosition(-5,-5);
    weaponPickupPoint.setOutlineColor(sf::Color(0,0,0,150));
    weaponPickupPoint.setOutlineThickness(3);

}
void Weapon::outOfScreenDraw(sf::RenderTarget& target, double x, double y, int mapX, int mapY, unsigned int vertical)
{
    double relativeRotation;
    double absX;
    double absY;
    double alpha = ((-sqrt((getX()-x)*(getX()-x) + (getY()-y)*(getY()-y))/sqrt(mapX*mapX + mapY*mapY))+1)*200 + 55;
    if(getX()-x < 0)
    {
        relativeRotation = atan((y-getY())/(getX()-x));
        absX = -(x-cos(relativeRotation)*(vertical/2 -40));
    }
    else
    {
        relativeRotation = atan(-(y-getY())/(getX()-x));
        absX = -(x+cos(relativeRotation)*(vertical/2 -40));
    }
    absY = -(y+sin(relativeRotation)*(vertical/2 -40));

    alternativeDraw.setOrigin(absX,absY);
    alternativeDraw.setFillColor(sf::Color(150,150,150,(int)alpha));
    alternativeDraw.setOutlineColor(sf::Color(0,0,0,(int)alpha));
    target.draw(alternativeDraw);
}

void Weapon::onGroundDraw(sf::RenderTarget& target)
{
    alternativeDraw.setPosition(getPosition());
    target.draw(alternativeDraw);

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
    return getPosition().x;
}
float Weapon::getY()
{
    return getPosition().y;
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

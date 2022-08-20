#include "Weapon.hpp"

Weapon::Weapon() : sf::Sprite()
{
}

void Weapon::loadWeapon(int i)
{
    type = i;
    std::string filename = "w"+ std::to_string(i)+".png";
    skin.loadFromFile(filename);
    useRotation = 45*i;
    range = 90 * i;
    cooldown = 500*i;
    power = 15*i;
    skin.setSmooth(true);
    setTexture(skin);
}
Weapon::Weapon(int type) : sf::Sprite()
{
    loadWeapon(type);

    alternativeDraw.setFillColor(sf::Color(150, 150, 150, 150));
    alternativeDraw.setRadius(10);
    alternativeDraw.setPosition(-10, -10);
    alternativeDraw.setOutlineColor(sf::Color(0, 0, 0, 150));
    alternativeDraw.setOutlineThickness(3);

    weaponPickupPoint.setFillColor(sf::Color(150, 150, 150, 150));
    weaponPickupPoint.setRadius(5);
    weaponPickupPoint.setPosition(-5, -5);
    weaponPickupPoint.setOutlineColor(sf::Color(0, 0, 0, 150));
    weaponPickupPoint.setOutlineThickness(3);
}
void Weapon::outOfScreenDraw(sf::RenderTarget &target, double x, double y, int mapX, int mapY, unsigned int vertical)
{
    double relativeRotation;
    double absX;
    double absY;
    double alpha = ((-sqrt((getX() - x) * (getX() - x) + (getY() - y) * (getY() - y)) / sqrt(mapX * mapX + mapY * mapY)) + 1) * 200 + 55;
    if (getX() - x < 0)
    {
        relativeRotation = atan((y - getY()) / (getX() - x));
        absX = -(x - cos(relativeRotation) * (vertical / 2 - 40));
    }
    else
    {
        relativeRotation = atan(-(y - getY()) / (getX() - x));
        absX = -(x + cos(relativeRotation) * (vertical / 2 - 40));
    }
    absY = -(y + sin(relativeRotation) * (vertical / 2 - 40));

    alternativeDraw.setOrigin(absX, absY);
    alternativeDraw.setFillColor(sf::Color(150, 150, 150, (int)alpha));
    alternativeDraw.setOutlineColor(sf::Color(0, 0, 0, (int)alpha));
    target.draw(alternativeDraw);
}

void Weapon::onGroundDraw(sf::RenderTarget &target)
{
    weaponPickupPoint.setPosition(getPosition());
    target.draw(weaponPickupPoint);
}

int Weapon::getUseRotation()
{
    return useRotation;
}

void Weapon::setPower(float p)
{
    power = p;
}
void Weapon::setRange(float r)
{
    range = r;
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

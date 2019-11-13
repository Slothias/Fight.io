#ifndef HPBAR_HPP
#define HPBAR_HPP

#include<SFML/Graphics.hpp>
class HpBar:public sf::Sprite
{
    public:
        HpBar(int m,float sx, float sy);
        void setMaxHp(int m);
        void setCurrentHp(int c);
        void draw(sf::RenderTarget& target, sf::RenderStates rs);
        void setPosition(float x, float y);
        void setColor(sf::Color color);
        void setMyLevel(int l);
        virtual ~HpBar();
    private:
        sf::Texture skin;
        sf::Sprite border;
        sf::RectangleShape hpBar;
        sf::Text* level;
        sf::Font font;
        int maxHp;
        int currentHp;
};

#endif // HPBAR_HPP

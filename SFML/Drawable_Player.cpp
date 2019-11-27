#include "Drawable_Player.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include<fstream>

Drawable_Player::Drawable_Player(std::string name,float x, float y, float a):sf::Sprite(),player(name,x,y,a)
{
    skin.loadFromFile("Player.png");
    font.loadFromFile("LiberationSans.ttf");
    myName = new sf::Text('<'+name+'>',font,12);
    myName->setColor(sf::Color::Black);
    myName->setStyle(sf::Text::Style::Bold);
    myName->setPosition(x-skin.getSize().x/2 + 5 , y - 126);
    myWeapon = nullptr;
    skin.setSmooth(true);
    me.setTexture(skin);
    setWeapon(weapon);
    myHpBar = new HpBar(maxHp,x-(skin.getSize().x/2), y-(1.5*skin.getSize().y));
    testHitbox.setFillColor(sf::Color(0,255,255,100));
    testHitbox.setRadius(hitboxRadius);
    testHitbox.setPosition(-hitboxRadius,-hitboxRadius);
    weaponHitbox.setFillColor(sf::Color::Red);
    weaponHitbox.setRadius(5);
    weaponHitbox.setOrigin(5,myWeapon->range+5);

    //setScale(0.5f,0.5f);
    setOrigin(skin.getSize().x/2, skin.getSize().y/2);
    setPosition(x,y,false);
    setRotation(a,false);
}

void Drawable_Player::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    target.draw(*myWeapon);
    target.draw(testHitbox);
    target.draw(me);
    myHpBar->draw(target,states);
    if(poking)
        target.draw(weaponHitbox);
    target.draw(*myName);

}
void Drawable_Player::setPosition(float x, float y,bool c)
{

    if(x!= playerX || y!=playerY)
    {
    my_mutex.lock();
    playerX=x;
    playerY=y;
    changed = c;
    myWeapon->setPosition(x,y);
    weaponHitbox.setPosition(x,y);
    //absolutePositionTester.setPosition(x+(cos((playerRotation-90)* 3.1415 / 180.0)*myWeapon->range),y+(sin((playerRotation-90)* 3.1415 / 180.0)*myWeapon->range));
    testHitbox.setOrigin(-x,-y);
    myHpBar->setPosition(x-(skin.getSize().x/2), y-(1.5*skin.getSize().y));
    me.setPosition(x,y);
    myName->setPosition(x-skin.getSize().x/2 + 5 , y - 126/*(1.5*skin.getSize().y + myName->getGlobalBounds().height)*/); ///nem szép, de nem hiszem hogy tovább kéne csavarni rajta
    my_mutex.unlock();
    }
}
void Drawable_Player::setRotation(float x, bool c)
{
    if(x!=playerRotation)
    {
    my_mutex.lock();
    playerRotation=x;
    weaponHitbox.setRotation(x);

    changed=c;
    me.setRotation(x);
    my_mutex.unlock();
    }
    if(!poking)
        myWeapon->setRotation(x);
    else
        myWeapon->setRotation(x-myWeapon->useRotation);
}
void Drawable_Player::testPoke(bool setToIt)  //próba a szurkálásra
{

    /*for(int i=0; i<=10; i++)
    {
        myWeapon->setRotation(me.getRotation()+((myWeapon->useRotation/10)*i));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    for(int i=10; i>=0; i--)
    {
        myWeapon->setRotation(me.getRotation()+(((myWeapon->useRotation)/10)*i));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }*/
    my_mutex.lock();
    poking = setToIt;
    changed=true;
    my_mutex.unlock();
}
void Drawable_Player::setScale(float x, float y)
{
    my_mutex.lock();
    me.setScale(x,y);
    myWeapon->setScale(x,y);
    my_mutex.unlock();
}
void Drawable_Player::setOrigin(float x, float y)
{
    my_mutex.lock();
    me.setOrigin(x,y);
    my_mutex.unlock();
}
void Drawable_Player::setCurrentHp(int _currentHp)
{
    my_mutex.lock();
    if(_currentHp <= maxHp)
    {
        currentHp = _currentHp;
        myHpBar->setCurrentHp(currentHp);
    }
    my_mutex.unlock();
}
void Drawable_Player::setWeapon(int _weapon)
{
    weapon = _weapon;
    if(myWeapon)
        delete myWeapon;
    myWeapon = new Weapon(weapon);
    myWeapon->setPosition(getPosition());
    if(weapon==0 || weapon==1 ||weapon==4)
        myWeapon->setOrigin(-(((int)skin.getSize().x/2)-(int)(myWeapon->getTexture()->getSize().x)), ((int)myWeapon->getTexture()->getSize().y+myWeapon->getTexture()->getSize().y/8));
    else
        myWeapon->setOrigin(-(((int)skin.getSize().x/2)-(int)(3*myWeapon->getTexture()->getSize().x/4)), ((int)myWeapon->getTexture()->getSize().y+myWeapon->getTexture()->getSize().y/15));
    myWeapon->setRotation(getRot());
    weaponHitbox.setOrigin(5,myWeapon->range+5);

}
//---------GETTERS---------------
Weapon* Drawable_Player::getWeapon()
{
    return myWeapon;
}
void Drawable_Player::update(std::string data)
{

    ///stringstream egyszer lesz létrehozva
    std::stringstream ss(data);
    ///megkeressük ki az üzenet feladója
    std::string currentName;
    std::getline(ss,currentName,':');
    ///ha exitet kaptunk
    if( data. find("EXIT")!=std::string::npos)
    {
        ///ha a szerver küldte az exitet, mindenki mehet a picsába
        if(currentName == "Server")
        {
            std::cout<<"Server exit"<<std::endl;
            std::map<std::string, Drawable_Player*> myMap =players;
            players.clear();
        }
        ///ha egy játékos küldött exitet, akkor csak ő mehet a picsába
        else
        {
            Drawable_Player* p = players[currentName];
            players.erase(currentName);
            delete p;
        }
    }
    ///egyébként frissítünk
    else if(data.find("|")!=std::string::npos)
    {
        std::string flags;
        std::string line;
        float curx,cury,getrot;
        bool curPoking=false;
        std::getline(ss,flags,'|');
        if(flags.at(1) == '1')
        {
            std::getline(ss,line,'|');
            curx = std::stof(line);
            std::getline(ss,line,'|');
            cury = std::stof(line);
        }
        if(flags.at(2) == '1')
        {
            std::getline(ss,line,'|');
            getrot = std::stof(line);
        }
        if(flags.at(3) == '1')
        {
            std::cout<<flags.substr(0,4);
            curPoking = true;
        }else{
            curPoking = false;
        }
        //curPoking=flags.at(3)=='1';
        std::getline(ss,line,'|');
        int maxhp = std::stoi(line);
        std::getline(ss,line,'|');
        int curhp = std::stoi(line);
        std::getline(ss,line,'|');
        int getscore = std::stoi(line);
        std::getline(ss,line,'|');
        int wp =std::stoi(line);
        my_mutex.lock();
        ///ha nincs meg ez a játékos, akkor hozzáadjuk
        if(players.find(currentName)== players.end())
        {
                if(pName!=currentName)
                    players[currentName] = new Drawable_Player(currentName,curx,cury,getrot);
        }
        else
        {
            if(currentName!=pName)
            {
            ///egyébként frissítjük
            Drawable_Player* act = players[currentName];
            ///ha eltér a pozíció,akkor frissít
            if(/*(act->getX()!=curx || act->getY()!=cury) && */flags.at(1) == '1')
                act->setPosition(curx,cury,false);
            ///ha eltér a szög,akkor frissít
            if(/*act->getRot()!=getrot && */flags.at(2) == '1')
                players[currentName]->setRotation(getrot,false);
            ///ha eltér a bökés, akkor frissít
            if(act->poking != curPoking)
                players[currentName]->poking = curPoking;
            ///ha eltér a maxhp,akkor frissít
            if(act->getMaxHp()!=maxhp)
                players[currentName]->setMaxHp(maxhp);
            ///ha eltér a currentHP,akkor frissit
            if(act->getCurrentHp()!=curhp)
                players[currentName]->setCurrentHp(curhp);
            ///ha eltér a score,akkor frissít
            if(act->getScore()!=getscore)
                players[currentName]->setScore(getscore);
            if(act->getWeapon()->type!=wp)
                players[currentName]->setWeapon(wp);
            }
            else
            {
                if(flags.at(1)=='1')
                    setPosition(curx,cury,false);
                if(flags.at(2)=='1')
                    setRotation(getrot,false);
                if(poking != curPoking)
                {

                    poking = curPoking;

                }
                if(getMaxHp()!=maxhp)
                    setMaxHp(maxhp);
                if(getCurrentHp()!=curhp)
                    setCurrentHp(curhp);
                if(getScore()!=getscore)
                    setScore(getscore);
                if(weapon!=wp)
                    setWeapon(wp);
                if(flags.at(3)=='1')
                    testPoke(curPoking);
            }
        }
    my_mutex.unlock();

    }
}
sf::Vector2<float> Drawable_Player::getPosition()
{
    return me.getPosition();
}
std::map<std::string, Drawable_Player*> Drawable_Player::getPlayers()
{
    my_mutex.lock();
    std::map<std::string,Drawable_Player*> result = players;
    my_mutex.unlock();
    return result;
}

sf::Texture Drawable_Player::getSkin()
{
    return skin;
}
Drawable_Player::~Drawable_Player()
{
    delete myWeapon;
    delete myHpBar;
    //dtor
}


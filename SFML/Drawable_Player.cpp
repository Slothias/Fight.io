#include "Drawable_Player.hpp"

Drawable_Player::Drawable_Player(std::string name,float x, float y, float a):sf::Sprite(),player(name,x,y,a)
{
    skin.loadFromFile("Player.png");
    myWeapon = nullptr;
    skin.setSmooth(true);
    me.setTexture(skin);
    setWeapon(weapon);
    myHpBar = new HpBar(maxHp,x,y);
    testHitbox.setFillColor(sf::Color(0,255,255,100));
    testHitbox.setRadius(hitboxRadius);
    testHitbox.setPosition(-hitboxRadius,-hitboxRadius);

    //setScale(0.5f,0.5f);
    setOrigin(skin.getSize().x/2, skin.getSize().y/2);
    setPosition(x,y);
    setRotation(a);
}

void Drawable_Player::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    target.draw(*myWeapon);
    target.draw(testHitbox);
    target.draw(me);
    myHpBar->draw(target,states);

}
void Drawable_Player::setPosition(float x, float y)
{
    my_mutex.lock();
    playerX=x;
    playerY=y;
    myWeapon->setPosition(x,y);
    testHitbox.setOrigin(-x,-y);
    myHpBar->setPosition(x-(skin.getSize().x/2), y-(1.5*skin.getSize().y));
    me.setPosition(x,y);
    my_mutex.unlock();
}
void Drawable_Player::setRotation(float x)
{
    my_mutex.lock();
    playerRotation=x;
    me.setRotation(x);
    myWeapon->setRotation(x);
    my_mutex.unlock();
}
void Drawable_Player::testPoke()  //próba a szurkálásra
{
    /*
    for(int i=0; i<=10; i++)
    {
        myWeapon->setRotation(me.getRotation+((myWeapon->useRotation/10)*i));
        //draw()
    }
    for(int i=10; i>=0; i++)
    {
        myWeapon->setRotation(me.getRotation+((myWeapon->useRotation/10)*i));
        //draw()
    }*/
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
        //  std::cout<<" NEW DATA "<<std::endl;
        //std::cout <<data<<std::endl;
        std::string line;
        std::getline(ss,line,'|');
        float curx = std::stof(line);
        std::getline(ss,line,'|');
        float cury = std::stof(line);
        std::getline(ss,line,'|');
        float getrot = std::stof(line);
        std::getline(ss,line,'|');
        int maxhp = std::stoi(line);
        std::getline(ss,line,'|');
        int curhp = std::stoi(line);
        std::getline(ss,line,'|');
        int getscore = std::stoi(line);
        std::getline(ss,line,'|');
        int wp = 1;

        my_mutex.lock();
        ///ha nincs meg ez a játékos, akkor hozzáadjuk
        if(players.find(currentName)== players.end())
        {
            players[currentName] = new Drawable_Player(currentName,curx,cury,getrot);
        }
        else
        {
            ///egyébként frissítjük
            Drawable_Player* act = players[currentName];
            ///ha eltér a pozíció,akkor frissít
            if(act->getX()!=curx || act->getY()!=cury)
                act->setPosition(curx,cury);
            ///ha eltér a szög,akkor frissít
            if(act->getRot()!=getrot)
                players[currentName]->setRotation(getrot);
            ///ha eltér a maxhp,akkor frissít
            if(act->getMaxHp()!=maxhp)
                players[currentName]->setMaxHp(maxhp);
            ///ha eltér a currentHP,akkor frissit
            if(act->getCurrentHp()!=curhp)
                players[currentName]->setCurrentHp(curhp);
            ///ha eltér a score,akkor frissít
            if(act->getScore()!=getscore)
                players[currentName]->setScore(getscore);
            //players[currentName]->setWeapon(wp);
        }

    }
    my_mutex.unlock();
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

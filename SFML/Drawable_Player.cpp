#include "Drawable_Player.hpp"
#include <chrono>
#include <thread>
#include<fstream>
#include<iostream>
Drawable_Player::Drawable_Player(std::string name,float x, float y, float a):sf::Sprite(),player(name,x,y,a)
{
    readCfg();
    weapons.resize(maxPlayers-1);
    for (int i=0; i<weapons.size(); i++)
    {
        weapons[i] = nullptr;
    }

    respawn=false;
    lastPoke = std::chrono::high_resolution_clock::now();
    skin.loadFromFile("Player.png");
    deadSkin.loadFromFile("DeadPlayer.png");
    font.loadFromFile("LiberationSans.ttf");
    myName = new sf::Text('<'+name+'>',font,12);
    myName->setColor(sf::Color::Black);
    myName->setStyle(sf::Text::Style::Bold);
    myName->setPosition(x-skin.getSize().x/2 + 5, y - 126);
    skin.setSmooth(true);
    myWeapon = nullptr;
    addWeapon(0,weapons);
    deadSkin.setSmooth(true);
    me.setTexture(skin);
    deadMe.setTexture(deadSkin);
    myHpBar = new HpBar(maxHp,x-(skin.getSize().x/2), y-(1.5*skin.getSize().y));
    testHitbox.setFillColor(sf::Color(0,255,255,100));
    testHitbox.setRadius(hitboxRadius);
    testHitbox.setPosition(-hitboxRadius,-hitboxRadius);
    alternativeDraw.setFillColor(sf::Color(255,0,0,150));
    alternativeDraw.setRadius(15);
    alternativeDraw.setPosition(-15,-15);
    alternativeDraw.setOutlineColor(sf::Color(0,0,0,150));
    alternativeDraw.setOutlineThickness(3);
    weaponHitbox.setFillColor(sf::Color(255,0,0,50));
    weaponHitbox.setRadius(5);
    weaponHitbox.setOrigin(5,myWeapon->range+5);
    me.setPosition(x,y);
    myWeapon->setPosition(x,y);
    weaponHitbox.setPosition(x,y);
    testHitbox.setOrigin(-x,-y);
    myHpBar->setPosition(x-(skin.getSize().x/2), y-(1.5*skin.getSize().y));
    //setScale(0.5f,0.5f);
    setOrigin(skin.getSize().x/2, skin.getSize().y/2);
    setPosition(x,y,true);
    deadMe.setPosition(x,y);
    setRotation(a,true);
}

void Drawable_Player::readCfg()
{
std::string path= "config.cfg";
    std::ifstream myfile(path.c_str());
    if(myfile.is_open())
    {
        std::string line;
        myfile>>line;
        myfile>>line;
        maxPlayers = stoi(line);
        myfile>>line;
        myfile>>line;
        mapSize = stod(line);
    }
    myfile.close();
}

int Drawable_Player::getMapSize()
{
    return mapSize;
}

void Drawable_Player::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    auto curTime = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::milliseconds>( curTime - lastPoke ).count();
    if(poking || duration < myWeapon->cooldown)
    {
        myWeapon->setRotation(playerRotation -myWeapon->useRotation + (myWeapon->useRotation*(duration/myWeapon->cooldown)));
        weaponHitbox.setFillColor(sf::Color(255,0,0,(int)(155*((-duration/myWeapon->cooldown)+1))+100));
    }
    else
    {
        myWeapon->setRotation(playerRotation);
        weaponHitbox.setFillColor(sf::Color(255,0,0,100));
    }
    if(currentHp > 0)
    {
        target.draw(weaponHitbox);
        target.draw(*myWeapon);
        myHpBar->draw(target,states);
        target.draw(*myName);
        target.draw(testHitbox);
        target.draw(me);
    }else{
        target.draw(deadMe);
    }


}
void Drawable_Player::outOfScreenDraw(sf::RenderTarget& target, double x, double y, int mapX, int mapY, unsigned int vertical)
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
    alternativeDraw.setFillColor(sf::Color(255,0,0,(int)alpha));
    alternativeDraw.setOutlineColor(sf::Color(0,0,0,(int)alpha));
    target.draw(alternativeDraw);
}
void Drawable_Player::setPosition(float x, float y,bool c)
{

    if(x!= playerX || y!=playerY)
    {
        my_mutex.lock();
        playerX=x;
        playerY=y;
        changed = c;
        me.setPosition(sf::Vector2f(x,y));
        myWeapon->setPosition(x,y);
        weaponHitbox.setPosition(x,y);
        testHitbox.setOrigin(-x,-y);
        myHpBar->setPosition(x-(skin.getSize().x/2), y-(1.5*skin.getSize().y));
        deadMe.setPosition(x,y);
        myName->setPosition(x-skin.getSize().x/2 + 5, y - 126);
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
        deadMe.setRotation(x);
        my_mutex.unlock();
    }
    if(!poking)
        myWeapon->setRotation(x);
    else
        myWeapon->setRotation(x-myWeapon->useRotation);
}
void Drawable_Player::testPoke(bool setToIt)
{
    if(setToIt)
    {
        auto curTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( curTime - lastPoke ).count();
        if(duration > myWeapon->cooldown)
        {
            my_mutex.lock();
            poking = setToIt;
            changed=true;
            lastPoke = std::chrono::high_resolution_clock::now();
            my_mutex.unlock();
        }
        else
        {
            my_mutex.lock();
            if(poking == setToIt || changed)
                changed=true;
            else
                changed=false;
            poking = false;
            my_mutex.unlock();
        }
    }
    else
    {
        my_mutex.lock();
        if(poking != setToIt || changed)
            changed=true;
        else
            changed=false;
        poking = setToIt;
        my_mutex.unlock();
    }
}
void Drawable_Player::pickUpEvent(bool setToIt)
{
    my_mutex.lock();
    if(setToIt || (setToIt != pickUp))
        changed = true;
    else
        changed = false;
    pickUp = setToIt;
    my_mutex.unlock();
}
void Drawable_Player::deleteWeapon(int i)
{
    for(std::map<std::string,Drawable_Player*>::iterator it=players.begin(); it!=players.end(); it++)
    {
        if(it->first!=pName)
        {
            Weapon* w = it->second->weapons.at(i);
            it->second->weapons[i]=nullptr;
            delete w;
        }
        else
        {
            weapons[i]=nullptr;
        }
    }
}

bool Drawable_Player::iThinkICanPickUp()
{
    double weaponX, weaponY;
    for(int i=0; i<weapons.size(); i++)
    {
        if(weapons[i] != nullptr){
            weaponX = weapons[i]->getPosition().x;
            weaponY = weapons[i]->getPosition().y;
            if(sqrt((playerX-weaponX)*(playerX-weaponX) + (playerY-weaponY)*(playerY-weaponY)) < hitboxRadius)
            {
                return true;
            }
        }
    }
    return false;
}

void Drawable_Player::setScale(float x, float y)
{
    my_mutex.lock();
    me.setScale(x,y);
    deadMe.setScale(x,y);
    myWeapon->setScale(x,y);
    my_mutex.unlock();
}
void Drawable_Player::setOrigin(float x, float y)
{
    my_mutex.lock();
    me.setOrigin(x,y);
    deadMe.setOrigin(x,y);
    my_mutex.unlock();
}
void Drawable_Player::setCurrentHp(int _currentHp)
{
    my_mutex.lock();
    if(_currentHp <= maxHp)
    {
        currentHp=_currentHp;
        myHpBar->setCurrentHp(_currentHp);
    }
    my_mutex.unlock();
}
void Drawable_Player::addWeapon(int i,std::vector<Weapon*>& p)
{
    sf::Vector2f mypos = getPosition();
    float myrot = getRot();
    my_mutex.lock();
    if(!myWeapon)
    {
        weaponpos = -1;
        weapon = i;
        myWeapon=new Weapon(i);
        myWeapon->setPosition(mypos);
        myWeapon->setOrigin(-(((int)skin.getSize().x/2)-(int)(3*myWeapon->getTexture()->getSize().x/4)), ((int)myWeapon->getTexture()->getSize().y+myWeapon->getTexture()->getSize().y/15));
        myWeapon->setRotation(myrot);
        weaponHitbox.setOrigin(5,myWeapon->range+5);
    }
    else if(p.at(i))
        {
            Weapon* w = myWeapon;
            myWeapon = p.at(i);
            weaponpos = i;
            weapon = myWeapon->type;
            p[i] = nullptr;
            myWeapon->setPosition(mypos);
            myWeapon->setOrigin(-(((int)skin.getSize().x/2)-(int)(3*myWeapon->getTexture()->getSize().x/4)), ((int)myWeapon->getTexture()->getSize().y+myWeapon->getTexture()->getSize().y/15));
            myWeapon->setRotation(myrot);
            weaponHitbox.setOrigin(5,myWeapon->range+5);
            delete w;
        }
    my_mutex.unlock();

}
void Drawable_Player::setLevel(int i)
{
    myHpBar->setMyLevel(i);
}

void Drawable_Player::setMaxHp(int _maxHp)
{
    maxHp = _maxHp;
    myHpBar->setMaxHp(maxHp);
}

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
    if(currentName.length()>0)
    {
    ///ha új fegyót kapunk
    if(currentName == "Server" && data.find("EXIT")==std::string::npos)
    {
        std::string line;
        int id,type;
        float posx,posy;
        std::getline(ss,line,'|');
        id = std::stoi(line);
        std::getline(ss,line,'|');
        type = std::stoi(line);
        std::getline(ss,line,'|');
        posx = std::stof(line);
        std::getline(ss,line);
        posy = std::stof(line);
        my_mutex.lock();
        weapons[id] = new Weapon(type);
        weapons[id]->setPosition(posx,posy);
        my_mutex.unlock();
    }

    ///ha exitet kaptunk
    if( data.find("EXIT")!=std::string::npos)
    {
        ///ha a szerver küldte az exitet, mindenki mehet a picsába
        if(currentName == "Server")
        {
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
    else if(data.find("|")!=std::string::npos && data.find("Server")==std::string::npos)
    {
        my_mutex.lock();
        std::string flags;
        std::string line;
        float curx=0;
        float cury=0;
        float getrot=0;
        int weaponID = -1;
        bool curPoking=false;
        std::getline(ss,flags,'|');
        std::cout<<currentName<<":"<<flags<<"--->"<<data<<std::endl;
        my_mutex.unlock();
        if(flags.length()==4)
        {
        if(flags.at(0) == '1')
        {
            std::getline(ss,line,'|');
            curx = std::stof(line);
            std::getline(ss,line,'|');
            cury = std::stof(line);
        }
        if(flags.at(1) == '1')
        {
            std::getline(ss,line,'|');
            getrot = std::stof(line);
        }
        if(flags.at(2) == '1')
        {
            curPoking = true;

        }
        else
        {
            curPoking = false;
        }
        if(flags.at(3) == '1')
        {
            std::getline(ss,line,'|');
            weaponID = std::stoi(line);
        }
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
        if(players.find(currentName)== players.end() && pName!=currentName)
        {
            players[currentName] = new Drawable_Player(currentName,curx,cury,getrot);
            my_mutex.unlock();
        }
        else
        {
            if(currentName!=pName)
            {
                ///egyébként frissítjük
                Drawable_Player* act = players[currentName];
                my_mutex.unlock();
                ///ha eltér a pozíció,akkor frissít
                if(flags.at(0) == '1')
                    act->setPosition(curx,cury,false);
                ///ha eltér a szög,akkor frissít
                if(flags.at(1) == '1')
                    players[currentName]->setRotation(getrot,false);
                ///ha eltér a bökés, akkor frissít
                if(act->poking != curPoking)
                {
                    players[currentName]->poking = curPoking;
                    players[currentName]->lastPoke = std::chrono::high_resolution_clock::now();
                }
                if(flags.at(3) == '1')
                {
                    players[currentName]->addWeapon(weaponID,weapons);
                }
                ///ha eltér a maxhp,akkor frissít
                if(act->getMaxHp()!=maxhp)
                   {
                     players[currentName]->setMaxHp(maxhp);
                   }
                ///ha eltér a currentHP,akkor frissit
                if(act->getCurrentHp()!=curhp)
                {
                    players[currentName]->setCurrentHp(curhp);
                    if(players[currentName]->getCurrentHp() == 0)
                        players[currentName]->setNewWeapon(0);
                }
                ///ha eltér a score,akkor frissít
                if(act->getScore()!=getscore)
                {
                    players[currentName]->setScore(getscore);
                    players[currentName]->setLevel(players[currentName]->getLevel());
                }
            }
            else
            {
                my_mutex.unlock();
                if(getMaxHp()!=maxhp)
                {
                    setMaxHp(maxhp);
                }
                if(flags.at(3) == '1')
                        {
                            addWeapon(weaponID,weapons);
                        }
                if(getCurrentHp()!=curhp)
                {
                    setCurrentHp(curhp);
                    if(getRespawn())
                    {
                        if(flags.at(0)=='1')
                            setPosition(curx,cury,false);
                        if(flags.at(1)=='1')
                            setRotation(getrot,false);
                        if(getPoke() != curPoking)
                        {
                            setPoke(curPoking);
                        }
                        setRespawn(false);
                    }
                    else if(curhp <= 0)
                    {
                        setNewWeapon(0);
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                        setRespawn(true);
                        setChange(true);
                    }
                }

                if(getScore()!=getscore)
                {
                    setScore(getscore);
                    setLevel(getLevel());
                }
            }
        }

    }
    }
    }
}
void Drawable_Player::setNewWeapon(int type)
{
    delete myWeapon;
    myWeapon = new Weapon(type);
    myWeapon->setPosition(getPosition());
    myWeapon->setOrigin(-(((int)skin.getSize().x/2)-(int)(3*myWeapon->getTexture()->getSize().x/4)), ((int)myWeapon->getTexture()->getSize().y+myWeapon->getTexture()->getSize().y/15));
    myWeapon->setRotation(getRot());
    weaponHitbox.setOrigin(5,myWeapon->range+5);
}
int Drawable_Player::getLevel()
{
    int ret;
    my_mutex.lock();
    ret = level;
    my_mutex.unlock();
    return ret;
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

std::vector<Weapon*> Drawable_Player::getWeapons()
{
    my_mutex.lock();
    std::vector<Weapon*> result = weapons;
    my_mutex.unlock();
    return result;
}
sf::Texture Drawable_Player::getSkin()
{
    return skin;
}
Drawable_Player::~Drawable_Player()
{
    delete myHpBar;
}

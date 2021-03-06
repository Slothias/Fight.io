#include "Drawable_Player.hpp"

Drawable_Player::Drawable_Player(std::string name,float x, float y, float a):sf::Sprite()
{
    pName=name;
    skin.loadFromFile("Player.png");
    me.setTexture(skin);
    myWeapon.setOrigin(-((int)skin.getSize().x/2), (myWeapon.skin.getSize().y/2)+50);
    setScale(0.5f,0.5f);
    setOrigin(skin.getSize().x/2, skin.getSize().y/2);
    setPosition(x,y);
    setRotation(a);
}

void Drawable_Player::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    target.draw(me);
    target.draw(myWeapon);

}
void Drawable_Player::setPosition(float x, float y)
{
    playerX=x;
    playerY=y;
    myWeapon.setPosition(x,y);
    me.setPosition(x,y);
}
void Drawable_Player::setRotation(float x)
{
    playerRotation=x;
    me.setRotation(x);
    myWeapon.setRotation(x);
}
void Drawable_Player::setScale(float x, float y)
{
    me.setScale(x,y);
    myWeapon.setScale(x,y);
}
void Drawable_Player::setOrigin(float x, float y)
{
    me.setOrigin(x,y);
}

void Drawable_Player::setMaxHp(int _maxHp)
{
    maxHp=_maxHp;
}
void Drawable_Player::setCurrentHp(int _currentHp)
{
    currentHp=_currentHp;
}
void Drawable_Player::setScore(int _score)
{
    score=_score;
}
void Drawable_Player::setWeapon(Weapon _weapon)
{
    myWeapon=_weapon;
}
//---------GETTERS---------------

float Drawable_Player::getX()
{
    return playerX;
}
float Drawable_Player::getY()
{
    return playerY;
}
float Drawable_Player::getRot()
{
    return playerRotation;
}
std::string Drawable_Player::getName()
{
    return pName;
}
int Drawable_Player::getMaxHp()
{
    return maxHp;
}
int Drawable_Player::getCurrentHp()
{
    return currentHp;
}

int Drawable_Player::getScore()
{
    return score;
}
Weapon Drawable_Player::getWeapon()
{
    return myWeapon;
}
std::string Drawable_Player::getMSG()
{
 std::stringstream s;
    my_mutex.lock();
    s << getX() << "|" << getY() << "|" << getRot();
my_mutex.unlock();
    return s.str();
}
std::string Drawable_Player::toString()
{
     std::stringstream s;
    s << "X: " << getX() << " Y: " << getY() << " R: " << getRot()
        << " PName: " << getName() << " maxHP: " << getMaxHp() << " currentHP: " << getCurrentHp()
        << " Score: " << getScore();
    return s.str();
}
void Drawable_Player::update(std::string data)
{
     if(data.find("Your Name:")!=std::string::npos)
        {
            //std::cout<<" NEW NAME!!! "<<std::endl;
            pName = data.substr(10, data.length()-1);
            players.insert(std::pair<std::string, Drawable_Player*>(pName,this));
        }
    else if( data. find("EXIT")!=std::string::npos)
    {
        std::stringstream ss(data);
        std::string currentName;
        std::getline(ss,currentName,':');
        Drawable_Player* p = players[currentName];
        players.erase(currentName);
        delete p;
    }
    else if(data.find("|")!=std::string::npos)
    {
      //  std::cout<<" NEW DATA "<<std::endl;
      //  std:: <<data<<std::endl;
        std::stringstream ss(data);
        std::string currentName;
        std::getline(ss,currentName,':');
        std::string line;
        std::getline(ss,line,'|');
        float curx = std::stof(line);
        std::getline(ss,line,'|');
        float cury = std::stof(line);
        std::getline(ss,line,'|');
        float getrot = std::stof(line);
        my_mutex.lock();
        if(players.find(currentName)== players.end())
        {
            players[currentName] = new Drawable_Player(currentName,curx,cury,getrot);
        }
        else
        {
        players[currentName]->setPosition(curx,cury);
        players[currentName]->setRotation(getrot);
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
    //dtor
}

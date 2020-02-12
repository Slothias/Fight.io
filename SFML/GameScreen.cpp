#include "GameScreen.hpp"
#include <vector>
#include<string>
#include<algorithm>
#include<sstream>
GameScreen::GameScreen(sf::RenderWindow *App, Client* my)
{
    app=App;
    font.loadFromFile("ARCADECLASSIC.TTF");
    deathFont.loadFromFile("youmurdererbb_reg.ttf");
    c=my;
    me=new Drawable_Player(my->getName(),0,0,0);
    c->addPlayer(me);
    initMe(c->getData());
    mapSize = me->getMapSize();

    std::thread t(&Client::runclient,&(*c));
    t.detach();

    pup=pdown=pleft=pright=pPoke=false;

    forBackground.loadFromFile("hexagonal.png");
    forBackground.setRepeated(true);
    background.setTexture(forBackground);
    background.setPosition(-mapSize/2,-mapSize/2);
    background.setTextureRect(sf::IntRect(0,0,mapSize,mapSize));

    GetDesktopResolution();

    screenCenter.x = horizontal/2;
    screenCenter.y = vertical/2;

    for(int i=0; i<5; i++)
    {
        scoreboard.push_back(sf::Text());
        scoreboard[i].setCharacterSize(20);
        scoreboard[i].setColor(sf::Color(0,0,0,200));
        scoreboard[i].setFont(font);
    }
    scoreboardBackground.setFillColor(sf::Color(150,150,150,150));
    scoreboardBackground.setSize(sf::Vector2f(300,scoreboard[0].getCharacterSize()*scoreboard.size() + 20));
    scoreboardBackground.setOutlineColor(sf::Color(0,0,0,150));
    scoreboardBackground.setOutlineThickness(3);

    deathOverlay.setSize(sf::Vector2f(mapSize,mapSize));
    deathOverlay.setPosition(-mapSize/2,-mapSize/2);
    deathOverlay.setFillColor(sf::Color(0,0,0,150));

    youDied.setString("YOU DIED");
    youDied.setCharacterSize(400);
    youDied.setFont(deathFont);
    youDied.setColor(sf::Color::Red);

    viewOffSet = getViewOffSet();

}
void GameScreen::initMe(std::string msg)
{
        std::stringstream ss(msg);
        std::string flags;
        std::string line;
        std::getline(ss,line,':');
        float curx,cury,getrot;
        int weaponID = -1;
        bool curPoking=false;
        std::getline(ss,flags,'|');
        if(flags.at(0)=='1')
        {
            std::getline(ss,line,'|');
            curx = std::stof(line);
            std::getline(ss,line,'|');
            cury = std::stof(line);
            me->setPosition(curx,cury,true);
        }
        if(flags.at(1)=='1')
        {
            std::getline(ss,line,'|');
            me->setRotation(std::stof(line),false);
        }
}

void GameScreen::GetDesktopResolution()
{
   RECT desktop;
   const HWND hDesktop = GetDesktopWindow();
   GetWindowRect(hDesktop, &desktop);
   horizontal=desktop.right;
   vertical = desktop.bottom;

}
void GameScreen::draw()
{
    if((app->getSize().x != horizontal || app->getSize().y!=vertical) && c)
    {

    if(c->getconnected())
        {
        app->setSize(sf::Vector2<unsigned int>(horizontal,vertical));
        app->setPosition(sf::Vector2<int>(0,0));
        app->create(sf::VideoMode(horizontal,vertical),"mari",sf::Style::Fullscreen);
        app->setFramerateLimit(120);
        v.setSize(horizontal,vertical);
        app->setMouseCursorVisible(false);
        }
    }
    app->clear(sf::Color::White);

    app->draw(background);
    if(c && c->getconnected())
    {
        viewOffSet = getViewOffSet();
        v.setCenter(me->getX()+viewOffSet.x,me->getY()+viewOffSet.y);
        app->setView(v);

        std::vector<Weapon*> weapons = me->getWeapons();
        for(int i=0; i<weapons.size(); i++)
        {
            if(weapons[i] != nullptr)
            {
                app->draw(*weapons[i]);
                if(weapons[i]->getX() < v.getCenter().x-(horizontal/2) ||
                   weapons[i]->getX() > v.getCenter().x+(horizontal/2) ||
                   weapons[i]->getY() < v.getCenter().y-(vertical/2) ||
                   weapons[i]->getY() > v.getCenter().y+(vertical/2))
                {
                    weapons[i]->outOfScreenDraw(*app,me->getX(),me->getY(), background.getTextureRect().width, background.getTextureRect().height,vertical);
                }
                weapons[i]->onGroundDraw(*app);
            }
        }
        std::map<std::string,Drawable_Player*> players = me->getPlayers();
        std::vector<std::pair <std::string,int> > forScoreboard;
        for(std::pair<std::string, Drawable_Player*> entries: players)
        {
            entries.second->draw(*app,sf::RenderStates::Default);
            forScoreboard.push_back(std::pair<std::string,int>(entries.second->getName(),entries.second->getScore()));
            if((entries.second->getX() < v.getCenter().x-(horizontal/2) ||
               entries.second->getX() > v.getCenter().x+(horizontal/2) ||
               entries.second->getY() < v.getCenter().y-(vertical/2) ||
               entries.second->getY() > v.getCenter().y+(vertical/2))&&
               entries.second->getCurrentHp() > 0)
            {
                entries.second->outOfScreenDraw(*app,me->getX(),me->getY(), background.getTextureRect().width, background.getTextureRect().height,vertical);
            }

        }
        me->draw(*app,sf::RenderStates::Default);
        forScoreboard.push_back(std::pair<std::string,int>(me->getName(),me->getScore()));
        if(me->getCurrentHp() == 0)
        {
            app->draw(deathOverlay);
            youDied.setPosition(v.getCenter().x-youDied.getGlobalBounds().width/2,v.getCenter().y-youDied.getGlobalBounds().height*1.4);
            app->draw(youDied);
        }
        if(forScoreboard.size()>=2)
            sort(forScoreboard.begin(), forScoreboard.end(), [](const std::pair<std::string,int>& l, const std::pair<std::string,int>& r) {
                                                            return l.second > r.second;
            });
        scoreboardBackground.setPosition(v.getCenter().x - horizontal/2 + 10, v.getCenter().y - vertical/2 + 10);
        app->draw(scoreboardBackground);
        for(int i=0; (i<scoreboard.size() && i<forScoreboard.size()); i++)
        {
            std::stringstream ss;
            ss << "#" << i+1 << " " << forScoreboard[i].first << ": " << forScoreboard[i].second << " pts";
            scoreboard[i].setString(ss.str());
            scoreboard[i].setPosition(v.getCenter().x - horizontal/2 + 20, v.getCenter().y - vertical/2 + i*scoreboard[i].getCharacterSize() + 20);
            app->draw(scoreboard[i]);
        }

    }
    else{
        sf::Text text;
        text.setString("Unable  to  connect");
        text.setCharacterSize(30);
        text.setFont(font);
        text.setPosition(v.getCenter().x-text.getGlobalBounds().width/2,v.getCenter().y-text.getGlobalBounds().height/2);
        app->draw(text);
    }
}

void GameScreen::handle(sf::Event& event)
{
if(event.type == sf::Event::Closed || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
    {

    c->closeConnection();
    c->notify();
    app->close();
    }
else
{
    if(me->getCurrentHp()>0 && app->hasFocus())
    {
        float playerX, playerY;
        if(event.type == sf::Event::KeyPressed){
            if((event.key.code == sf::Keyboard::W)){
                pup = true;
            }
            if((event.key.code == sf::Keyboard::S)){
                pdown = true;
            }
            if((event.key.code == sf::Keyboard::A)){
                pleft = true;
            }
            if((event.key.code == sf::Keyboard::D)){
                pright = true;
            }
            if(event.key.code == sf::Keyboard::Q)
            {
                tempWeaponCounter ++;
                if(tempWeaponCounter > 5)
                {
                    tempWeaponCounter = 0;
                }
                me->setWeapon(tempWeaponCounter,true);
                me->weaponHitbox.setOrigin(5,me->getWeapon()->range+5);
                c->notify();
            }
    }
    if(sf::Joystick::isConnected(0))
    {
        if(sf::Joystick::getAxisPosition(0,sf::Joystick::Y)<-10){
            pup = true;
        }else{
            pup = false;
        }

        if(sf::Joystick::getAxisPosition(0,sf::Joystick::Y)>10){
            pdown = true;
        }else{
            pdown = false;
        }

        if(sf::Joystick::getAxisPosition(0,sf::Joystick::X)<-10){
            pleft = true;
        }else{
            pleft = false;
        }

        if(sf::Joystick::getAxisPosition(0,sf::Joystick::X)>10){
            pright = true;
        }else{
            pright = false;
        }

    }

        //Azért kell, hogy ne mehessen ki. A keypress eventnél nem vizsgálhatjuk, mert nincs mindig key press event.
            if((me->getY() < -background.getTextureRect().height/2) && pup)
            {
                pup = false;
            }
            if((me->getY() > background.getTextureRect().height/2) && pdown)
            {
                pdown = false;
            }
            if((me->getX() < -background.getTextureRect().width/2) && pleft)
            {
                pleft = false;
            }
            if((me->getX() > background.getTextureRect().width/2) && pright)
            {
                pright = false;
            }

            if((event.type == sf::Event::KeyReleased)){
                if((event.key.code == sf::Keyboard::W)){
                    pup = false;
                }
                if((event.key.code == sf::Keyboard::S)){
                    pdown = false;
                }
                if((event.key.code == sf::Keyboard::A)){
                    pleft = false;
                }
                if((event.key.code == sf::Keyboard::D)){
                    pright = false;
                }
            }

            playerX = 0;
            playerY = 0;
            if(pup && !pleft && !pright)
                {
                    //if(pályán belülre)
                    if(playerY - PLAYERMOVESPEED/SLEEPVAL > -background.getTextureRect().height/2)
                        playerY -= PLAYERMOVESPEED/SLEEPVAL;
                    else
                        playerY = -background.getTextureRect().height/2;
                }
            if(pdown && !pleft && !pright)
                {
                    //if(pályán belülre)
                    if(playerY + PLAYERMOVESPEED/SLEEPVAL < background.getTextureRect().height/2)
                        playerY += PLAYERMOVESPEED/SLEEPVAL;
                    else
                        playerY = background.getTextureRect().height/2;
                }
            if(pleft && !pup && !pdown)
                {
                    //if(pályán belülre)
                    if(playerX - PLAYERMOVESPEED/SLEEPVAL > -background.getTextureRect().width/2)
                        playerX -= PLAYERMOVESPEED/SLEEPVAL;
                    else
                        playerX = -background.getTextureRect().width/2;
                }
            if(pright && !pup && !pdown)
                {
                    //if(pályán belülre)
                    if(playerX + PLAYERMOVESPEED/SLEEPVAL < background.getTextureRect().width/2)
                        playerX += PLAYERMOVESPEED/SLEEPVAL;
                    else
                        playerX = background.getTextureRect().width/2;
                }

            //ezekbe már nem lép be, ha a pályának valamelyik szélén van
            if(pup && (pleft || pright)){playerY -= PLAYERMOVESPEED/(ROOT2*SLEEPVAL);}
            if(pdown && (pleft || pright)){playerY += PLAYERMOVESPEED/(ROOT2*SLEEPVAL);}
            if(pleft && (pup || pdown)){playerX -= PLAYERMOVESPEED/(ROOT2*SLEEPVAL);}
            if(pright && (pup || pdown)){playerX += PLAYERMOVESPEED/(ROOT2*SLEEPVAL);}

            float mousePosX = sf::Mouse::getPosition(*app).x;
            float mousePosY = sf::Mouse::getPosition(*app).y;

            viewOffSet = getViewOffSet();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                    pPoke = true;
                if(event.mouseButton.button == sf::Mouse::Right)
                {
                    if(me->iThinkICanPickUp())
                        me->pickUpEvent(true);
                    else
                        me->pickUpEvent(false);
                }
            }
            if(sf::Joystick::isConnected(0))
            {
                if(sf::Joystick::getAxisPosition(0,sf::Joystick::Z) > 20)
                    pPoke = true;
                else
                    pPoke = false;

                if(sf::Joystick::isButtonPressed(0,1))
                {
                    if(me->iThinkICanPickUp())
                        me->pickUpEvent(true);
                    else
                        me->pickUpEvent(false);
                }

            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                    pPoke = false;
                if(event.mouseButton.button == sf::Mouse::Right)
                    me->pickUpEvent(false);
            }

            me->testPoke(pPoke);

            /*
            if(!((horizontal/2 - viewOffSet.x)-mousePosX ==0)){
                if((horizontal/2 - viewOffSet.x)-mousePosX <= 0)
                    me->setRotation((atan(((vertical/2 - viewOffSet.y)-mousePosY)/((horizontal/2 - viewOffSet.x)-mousePosX)))/PI *180 +90,true);
                else
                    me->setRotation(((atan(((vertical/2 - viewOffSet.y)-mousePosY)/((horizontal/2 - viewOffSet.x)-mousePosX)))/PI *180) +270,true);
            }
            */

            if(sqrt((mousePosX-screenCenter.x)*(mousePosX-screenCenter.x)+(mousePosY-screenCenter.y)*(mousePosY-screenCenter.y))>10 ||
               (sf::Joystick::isConnected(0) && sqrt(sf::Joystick::getAxisPosition(0,sf::Joystick::V)*sf::Joystick::getAxisPosition(0,sf::Joystick::V) + sf::Joystick::getAxisPosition(0,sf::Joystick::U)*sf::Joystick::getAxisPosition(0,sf::Joystick::U))>50))
            {
                std::cout << "joy: " << sqrt(sf::Joystick::getAxisPosition(0,sf::Joystick::V)*sf::Joystick::getAxisPosition(0,sf::Joystick::V) + sf::Joystick::getAxisPosition(0,sf::Joystick::U)*sf::Joystick::getAxisPosition(0,sf::Joystick::U)) << std::endl;
                float target;
                float curX,curY;
                if(sf::Joystick::isConnected(0) == false)
                {
                    curX = screenCenter.x - mousePosX;
                    curY = screenCenter.y - mousePosY;
                }else{
                    curX = -sf::Joystick::getAxisPosition(0,sf::Joystick::U);
                    curY = -sf::Joystick::getAxisPosition(0,sf::Joystick::V);
                }

                if(curX != 0){
                        if(curX < 0)
                            //me->setRotation((atan((screenCenter.y-mousePosY)/(screenCenter.x-mousePosX)))/PI *180 +90,true);
                            target = (atan(curY/curX))/PI *180 +90;
                        else
                            //me->setRotation((atan((screenCenter.y-mousePosY)/(screenCenter.x-mousePosX)))/PI *180 +270,true);
                            target = (atan(curY/curX))/PI *180 +270;
                    }else{
                        if(curY > 0)
                            //me->setRotation(0,true);
                            target = 0;
                        else
                            //me->setRotation(180,true);
                            target = 180;
                    }
                me->setRotation(target,true);

                if(app->hasFocus())
                    sf::Mouse::setPosition(screenCenter);
            }

            float curx = me->getX();
            float cury = me->getY();
            me->setPosition(curx+playerX, cury+playerY,true);
        }else
        {
            pup = pdown = pleft = pright = pPoke = false;
        }
    if(me->getChange())
        c->notify();

    }
}

sf::Vector2<float> GameScreen::getViewOffSet()
{
    float viewOffsetX=0, viewOffsetY=0;
    //horizontális offset vizsgálat
    if(me->getX()-(horizontal/2) < -(background.getTextureRect().width/2))
    {
        viewOffsetX = (horizontal/2)-(me->getX()+(background.getTextureRect().width/2));
    }
    if(me->getX()+(horizontal/2) > background.getTextureRect().width/2)
    {
        viewOffsetX = (background.getTextureRect().width/2)-me->getX()-(horizontal/2);
    }
    // vertikális offset vizsgálat
    if(me->getY()-(vertical/2) < -(background.getTextureRect().height/2))
    {
        viewOffsetY = (vertical/2)-(me->getY()+(background.getTextureRect().height/2));
    }
    if(me->getY()+(vertical/2) > background.getTextureRect().height/2)
    {
        viewOffsetY = (background.getTextureRect().height/2)-me->getY()-(vertical/2);
    }
    return(sf::Vector2<float>(viewOffsetX,viewOffsetY));
}

GameScreen::~GameScreen()
{
    delete c;
    delete me;
}

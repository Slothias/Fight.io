#include "GameScreen.hpp"
GameScreen::GameScreen(sf::RenderWindow *App, const char* host)
{
    app=App;
    font.loadFromFile("ARCADECLASSIC.TTF");
    me=new Drawable_Player("Peti",0,0,0);
    pup=pdown=pleft=pright=false;
    forBackground.loadFromFile("hexagonal.png");
    forBackground.setRepeated(true);
    background.setTexture(forBackground);
    background.setPosition(-1500,-1000);
    background.setTextureRect(sf::IntRect(0,0,3000,2000));
    c=new Client(host,10043,me);
    std::thread t(&Client::runclient,&(*c));
    t.detach();
    GetDesktopResolution();
    tempWeaponCounter =0;
    viewOffSet = getViewOffSet();

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
        }
    }
    app->clear(sf::Color::White);

    app->draw(background);
    if(c && c->getconnected())
    {
    viewOffSet = getViewOffSet();
    v.setCenter(me->getX()+viewOffSet.x,me->getY()+viewOffSet.y);
    app->setView(v);
    std::map<std::string,Drawable_Player*> players = me->getPlayers();
    me->draw(*app,sf::RenderStates::Default);
    for(std::pair<std::string, Drawable_Player*> entries: players)
    {
        entries.second->draw(*app,sf::RenderStates::Default);
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
    app->close();
    }
else
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
            me->setWeapon(tempWeaponCounter);
            c->notify();
        }
        if(event.key.code==sf::Keyboard::E)
        {
            me->setCurrentHp(me->getCurrentHp()-1);
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

        if(!((horizontal/2 - viewOffSet.x)-mousePosX ==0)){
            if((horizontal/2 - viewOffSet.x)-mousePosX <= 0)
                me->setRotation((atan(((vertical/2 - viewOffSet.y)-mousePosY)/((horizontal/2 - viewOffSet.x)-mousePosX)))/PI *180 +90);
            else
                me->setRotation(((atan(((vertical/2 - viewOffSet.y)-mousePosY)/((horizontal/2 - viewOffSet.x)-mousePosX)))/PI *180) +270);
            c->notify();
        }


        float curx = me->getX();
        float cury = me->getY();
        me->setPosition(curx+playerX, cury+playerY);
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

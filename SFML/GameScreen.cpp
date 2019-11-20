#include "GameScreen.hpp"
GameScreen::GameScreen(sf::RenderWindow *App, const char* host)
{
    app=App;
    font.loadFromFile("ARCADECLASSIC.TTF");
    app->setFramerateLimit(FRAMERATE);
    me=new Drawable_Player("Peti",100,100,0);
    pup=pdown=pleft=pright=false;
    forBackground.loadFromFile("hexagonal.png");
    forBackground.setRepeated(true);
    background.setTexture(forBackground);
    background.setPosition(-2000,-2000);
    background.setTextureRect(sf::IntRect(0,0,4000,4000));
    c=new Client(host,10043,me);
    std::thread t(&Client::runclient,&(*c));
    t.detach();
    GetDesktopResolution();
    tempWeaponCounter =0;
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
        v.setSize(horizontal,vertical);
        }
    }
    app->clear(sf::Color::White);

    float viewOffsetX=0, viewOffsetY=0;
    //horizont�lis offset vizsg�lat
    if(me->getX()-(horizontal/2) < -(background.getTextureRect().width/2))
    {
        viewOffsetX = (horizontal/2)-(me->getX()+(background.getTextureRect().width/2));
    }
    if(me->getX()+(horizontal/2) > background.getTextureRect().width/2)
    {
        viewOffsetX = (background.getTextureRect().width/2)-me->getX()-(horizontal/2);
    }
    // vertik�lis offset vizsg�lat
    if(me->getY()-(vertical/2) < -(background.getTextureRect().height/2))
    {
        viewOffsetY = (vertical/2)-(me->getY()+(background.getTextureRect().height/2));
    }
    if(me->getY()+(vertical/2) > background.getTextureRect().height/2)
    {
        viewOffsetY = (background.getTextureRect().height/2)-me->getY()-(vertical/2);
    }

    /*if((me->getX()-horizontal/2 > -2000 && me->getX()+horizontal/2 < background.getTextureRect().width-2000) ||
        (me->getY()-vertical/2> -2000 && me->getY()+vertical/2<background.getTextureRect().height-2000))*/
    v.setCenter(me->getX()+viewOffsetX,me->getY()+viewOffsetY);
    app->setView(v);
    app->draw(background);
    if(c && c->getconnected())
    {
    std::map<std::string,Drawable_Player*> players = me->getPlayers();
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
                if(event.key.code == sf::Keyboard::Q){
                    tempWeaponCounter ++;
                    if(tempWeaponCounter > 5)
                        tempWeaponCounter = 0;
                    std::cout<<tempWeaponCounter<<std::endl;
                    me->setWeapon(tempWeaponCounter);
                }
                if(event.key.code==sf::Keyboard::E)
                {
                    me->setCurrentHp(me->getCurrentHp()-1);
                }
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
        if(pup && !pleft && !pright){playerY -= PLAYERMOVESPEED/SLEEPVAL;}
        if(pdown && !pleft && !pright){playerY += PLAYERMOVESPEED/SLEEPVAL;}
        if(pleft && !pup && !pdown){playerX -= PLAYERMOVESPEED/SLEEPVAL;}
        if(pright && !pup && !pdown){playerX += PLAYERMOVESPEED/SLEEPVAL;}

        if(pup && (pleft || pright)){playerY -= PLAYERMOVESPEED/(ROOT2*SLEEPVAL);}
        if(pdown && (pleft || pright)){playerY += PLAYERMOVESPEED/(ROOT2*SLEEPVAL);}
        if(pleft && (pup || pdown)){playerX -= PLAYERMOVESPEED/(ROOT2*SLEEPVAL);}
        if(pright && (pup || pdown)){playerX += PLAYERMOVESPEED/(ROOT2*SLEEPVAL);}

         float mousePosX = sf::Mouse::getPosition(*app).x;
        float mousePosY = sf::Mouse::getPosition(*app).y;
        if(!(app->getSize().x/2-mousePosX ==0)){
            if((app->getSize().x/2)-mousePosX <= 0)
                me->setRotation((atan(((app->getSize().y/2)-mousePosY)/((app->getSize().x/2)-mousePosX)))/PI *180 +90);
            else
                me->setRotation(((atan(((app->getSize().y/2)-mousePosY)/((app->getSize().x/2)-mousePosX)))/PI *180) +270);
        }


        float curx = me->getX();
        float cury = me->getY();
        if((curx+playerX> -2000 && curx+playerX<= background.getTextureRect().width-2000) || ( cury+playerY> -2000 && cury+playerY  <background.getTextureRect().height-2000))
            me->setPosition(curx+playerX, cury+playerY);

}
}

GameScreen::~GameScreen()
{
    delete c;
    delete me;
}

#include "GameScreen.hpp"

GameScreen::GameScreen(sf::RenderWindow *App)
{
    app=App;
    font.loadFromFile("ARCADECLASSIC.TTF");
    app->setFramerateLimit(FRAMERATE);
    me=new player("En");
    music.openFromFile("retro_menu.wav");
    pup=pdown=pleft=pright=false;
}

void GameScreen::draw()
{
    app->clear(sf::Color::White);
    v.setCenter(me->getX(),me->getY());
    app->setView(v);
    if(c && c->getconnected())
    {
        std::map<std::string,player*> players = me->getPlayers();
        for(std::pair<std::string, player*> entries: players)
        {
            app->draw(entries.second->getSprite());
            app->draw(*entries.second->getWeapon());
        }
    }
    else{
        sf::Text text;
        text.setString("Unable  to  connect");
        text.setCharacterSize(30);
        text.setColor(sf::Color::Red);
        text.setFont(font);
        text.setPosition(v.getCenter().x-text.getGlobalBounds().width/2,v.getCenter().y-text.getGlobalBounds().height/2);
        app->draw(text);
    }
}
sf::Music& GameScreen::getMusic()
{
    return music;
}

void GameScreen::handle(sf::Event event)
{

if(!c)
    {
        c=new Client("127.0.0.1",10043,me);
        std::thread t(&Client::runclient,&(*c));
        t.detach();
    }
if(event.type == sf::Event::Closed)
    c->closeConnection();
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
        if(pup && !pleft && !pright){playerY -= PLAYERMOVESPEED;}
        if(pdown && !pleft && !pright){playerY += PLAYERMOVESPEED;}
        if(pleft && !pup && !pdown){playerX -= PLAYERMOVESPEED;}
        if(pright && !pup && !pdown){playerX += PLAYERMOVESPEED;}

        if(pup && (pleft || pright)){playerY -= PLAYERMOVESPEED/ROOT2;}
        if(pdown && (pleft || pright)){playerY += PLAYERMOVESPEED/ROOT2;}
        if(pleft && (pup || pdown)){playerX -= PLAYERMOVESPEED/ROOT2;}
        if(pright && (pup || pdown)){playerX += PLAYERMOVESPEED/ROOT2;}

         float mousePosX = sf::Mouse::getPosition(*app).x;
        float mousePosY = sf::Mouse::getPosition(*app).y;
            if((app->getSize().x/2)-mousePosX < 0)
                me->setRotation((atan(((app->getSize().y/2)-mousePosY)/((app->getSize().x/2)-mousePosX)))/PI *180 +90);
            else
                me->setRotation(((atan(((app->getSize().y/2)-mousePosY)/((app->getSize().x/2)-mousePosX)))/PI *180) +270);


        me->w->setRotation(me->getRot());
        me->setPosition(me->getX()+playerX, me->getY()+playerY);
        me->w->setPosition(me->getX(),me->getY());

}
}

GameScreen::~GameScreen()
{
    //dtor
}

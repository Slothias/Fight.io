#include "GameScreen.hpp"
GameScreen::GameScreen(sf::RenderWindow *App)
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
    c=nullptr;
}
void GameScreen::draw()
{
    app->clear(sf::Color::White);
    v.setCenter(me->getX(),me->getY());
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

if(!c)
    {

        c=new Client("127.0.0.1",10043,me);
        std::thread t(&Client::runclient,&(*c));
        t.detach();
    }
else if(event.type == sf::Event::Closed)
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


        me->setPosition(me->getX()+playerX, me->getY()+playerY);

}
}

GameScreen::~GameScreen()
{
    delete c;
    delete me;
}

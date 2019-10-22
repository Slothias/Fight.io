#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>
#include "player.hpp"
#include "Client.hpp"

int main()
{
    // Define some constants
    const int framerate = 120;
    const int sleepVal = 1000/framerate;
    const float pi = 3.14159f;
    const float root2 = 1.41421356f;
    const int gameWidth = 800;
    const int gameHeight = 800;
    const float playerMoveSpeed = 1500;






    //Creating player
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("Player.png")){
        std::cout << "picture didn't load" << std::endl;
    }

    player* playerTester = new player("tester", 0, 0, 0, playerTexture);

    //setting up the connection
    Client c("127.0.0.1",10043,playerTester);
    std::thread t(&Client::runclient,&c);

    //Creating spear
    sf::Sprite spear;
    sf::Texture spearTexture;
    if (!spearTexture.loadFromFile("Spearr.png")){
        std::cout << "picture didn't load" << std::endl;
    }
    spear.setScale(sf::Vector2f(0.5f, 0.5f));
    //spear.setPosition((gameWidth/2)+50, gameHeight/2);
    spear.setPosition(playerTester->getX(),playerTester->getY());
    spear.setOrigin(-((int)playerTexture.getSize().x/2), (spearTexture.getSize().y/2)+30);

    spear.setTexture(spearTexture);
    std::cout << (int)(playerTexture.getSize().x/2)*-1 << std::endl;
    std::cout << spear.getPosition().x << " " << spear.getPosition().y << " " << spear.getOrigin().x << " " << spear.getOrigin().y << std::endl;

    /*sf::CircleShape porigin;
    porigin.setFillColor(sf::Color::Red);
    porigin.setRadius(5);
    porigin.setPosition(playerTester.getX(),playerTester.getY());
    porigin.setOrigin(2.5,2.5);

    sf::CircleShape sorigin;
    sorigin.setFillColor(sf::Color::Blue);
    sorigin.setRadius(3);
    porigin.setOrigin(1.5,1.5);
    sorigin.setPosition(spear.getPosition().x,spear.getPosition().y);*/


    //testing circle
    float circlex(200);
    float circley(200);
    sf::CircleShape circle;
    circle.setFillColor(sf::Color::Green);
    circle.setRadius(50);
    circle.setPosition(circlex,circley);

      // Create the window of the application

      sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "Character Testing",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
     sf::View View(window.getDefaultView());
    float mousePosX = sf::Mouse::getPosition(window).x;
    float mousePosY = sf::Mouse::getPosition(window).y;
    float prevMousePosX = mousePosX;
    float prevMousePosY = mousePosY;

    sf::Clock clock;

    bool pup(false), pdown(false), pleft(false), pright(false);
    //std::thread t(&EventHandler::runEventHandler, new EventHandler(sleepVal,framerate,&player,&playerX,&playerY,&pup,&pdown,&pleft,&pright,&playerRotation, &mousePosX, &mousePosY));
    float playerX, playerY;

    while (window.isOpen() /*&& c.getconnected()*/)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) ||
               ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                c.closeConnection();
                window.close();
                //playerTester = nullptr;
                break;
            }
            prevMousePosX = mousePosX;
            mousePosX = sf::Mouse::getPosition(window).x;
            prevMousePosY = mousePosY;
            mousePosY = sf::Mouse::getPosition(window).y;
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



        }
        playerX = 0;
        playerY = 0;
        if(pup && !pleft && !pright){playerY = -playerMoveSpeed/framerate;}
        if(pdown && !pleft && !pright){playerY = playerMoveSpeed/framerate;}
        if(pleft && !pup && !pdown){playerX = -playerMoveSpeed/framerate;}
        if(pright && !pup && !pdown){playerX = playerMoveSpeed/framerate;}

        if(pup && (pleft || pright)){playerY = -(playerMoveSpeed/framerate)/root2;}
        if(pdown && (pleft || pright)){playerY = (playerMoveSpeed/framerate)/root2;}
        if(pleft && (pup || pdown)){playerX = -(playerMoveSpeed/framerate)/root2;}
        if(pright && (pup || pdown)){playerX = (playerMoveSpeed/framerate)/root2;}

        playerTester->setPosition(playerX, playerY);

        if(mousePosX != prevMousePosX || mousePosY != prevMousePosY){
            if((gameWidth/2)-mousePosX < 0)
                playerTester->setRotation((atan(((gameHeight/2)-mousePosY)/((gameWidth/2)-mousePosX)))/pi *180 +90);
            else
                playerTester->setRotation(((atan(((gameHeight/2)-mousePosY)/((gameWidth/2)-mousePosX)))/pi *180) +270);
        }

        spear.setRotation(playerTester->getRot());
        spear.setPosition(playerTester->getX(),playerTester->getY());

        //player.setPosition(playerX,playerY);
        window.clear(sf::Color::White);
        View.setCenter(playerTester->getX(),playerTester->getY());
        //View.move(playerTester.getX(),playerTester.getY());
        window.setView(View);
        window.draw(spear);
        window.draw(playerTester->getSprite());
        window.draw(circle);
        //window.draw(porigin);
        //window.draw(sorigin);
        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(sleepVal));
    }
    t.join();
    return 0;

}

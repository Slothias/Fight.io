#include "ServerGui.hpp"

ServerGui::ServerGui():sf::RenderWindow(sf::VideoMode(800,600),"Server")
{
    is_running=false;
    server=nullptr;
    font.loadFromFile("LiberationSans.ttf");
    resultText.setFont(font);
    resultText.setString("DISCONNECTED");
    resultText.setPosition(getSize().x/2-resultText.getGlobalBounds().width/2,getSize().y/2-50);
    resultText.setColor(sf::Color::Red);
    errorText.setFont(font);
    setFramerateLimit(30);
    ipText.setFont(font);
    ipText.setColor(sf::Color::Blue);



    myTheme=simplgui::Theme::defaultTheme();
    auto resGetter = simplgui::FileResourcesGetter::create();
    button = simplgui::Button::create(resGetter);
    button = simplgui::Button::create(resGetter);
    button->setLabel(U"Connect");

    button->setPosition(sf::Vector2f(getSize().x/2-(button->getLabel()).size()*9,getSize().y/2));
    button->setSize(sf::Vector2f((button->getLabel()).size()*18,48));
    button->setTheme(myTheme);

    server = new Server();

    button->onClicked.bind([&](simplgui::Button::Ptr button)
    {

        if(!is_running)
        {
        try
        {
            server->tryToConnect();
            is_running=true;
            std::thread t([&](){
                          server->runServer();
                           // std::cout<<"futok"<<std::endl;
                          });
            t.detach();
        }
        catch (std::exception& e)
        {

            is_running=false;
            errorText.setString(e.what());
            errorText.setColor(sf::Color::Red);
            errorText.setPosition(getSize().x/2-errorText.getGlobalBounds().width/2,getSize().y/2+100);
            resultText.setColor(sf::Color::Red);
        }

        if(is_running)
        {
            resultText.setString("CONNECTED");
            resultText.setColor(sf::Color::Green);
            button->setLabel(U"Disconnect");
            resultText.setPosition(getSize().x/2-resultText.getGlobalBounds().width/2,getSize().y/2-50);
            button->setPosition(sf::Vector2f(getSize().x/2-(button->getLabel()).size()*8,getSize().y/2));
            button->setSize(sf::Vector2f((button->getLabel()).size()*16,48));
            ipText.setString(server->showStatus());
            ipText.setPosition(getSize().x/2-ipText.getGlobalBounds().width/2,getSize().y/2-80);
        }
        else
        {
            resultText.setString("DISCONNECTED");
            resultText.setColor(sf::Color::Red);
            resultText.setPosition(getSize().x/2-resultText.getGlobalBounds().width/2,getSize().y/2-50);
            button->setPosition(sf::Vector2f(getSize().x/2-(button->getLabel()).size()*8,getSize().y/2));
            button->setSize(sf::Vector2f((button->getLabel()).size()*16,48));
            ipText.setString("");

        }
        }
        else if(is_running)
        {
            server->closeServer();
            is_running=false;
            resultText.setString("DISCONNECTED");
            resultText.setColor(sf::Color::Red);
            button->setLabel(U"Connect");
            resultText.setPosition(getSize().x/2-resultText.getGlobalBounds().width/2,getSize().y/2-50);
            button->setPosition(sf::Vector2f(getSize().x/2-(button->getLabel()).size()*8,getSize().y/2));
            button->setSize(sf::Vector2f((button->getLabel()).size()*16,48));
            ipText.setString("");
            errorText.setString("");
        }
    });


}
void ServerGui::loop()
{
    while(isOpen())
    {
        clear(sf::Color::White);
        sf::Event event;
        auto dt = clock.restart();
        while(pollEvent(event))
        {
            button->update(dt);
            if(event.type == sf::Event::Closed)
                {
                    server->closeServer();
                    close();
                }
            button->processEvent(simplgui::Event(event, *this));
        }


          if(!is_running)
            draw(errorText);
        draw(resultText);
        draw(*button);
        draw(ipText);
        display();
    }
}

ServerGui::~ServerGui()
{
delete server;
//std::cout<<"GUI SHUTDOWN"<<std::endl;
}

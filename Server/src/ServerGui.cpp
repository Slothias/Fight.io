#include "ServerGui.hpp"

ServerGui::ServerGui() : sf::RenderWindow(sf::VideoMode(800, 600), "Fight.IO server")
{
    is_running = false;
    server = nullptr;
    font.loadFromFile("resources/fonts/LiberationSans.ttf");
    resultText.setFont(font);
    resultText.setString("DISCONNECTED");
    resultText.setPosition(getSize().x / 2 - resultText.getGlobalBounds().width / 2, getSize().y / 2 - 50);
    resultText.setFillColor(sf::Color::Red);
    errorText.setFont(font);
    setFramerateLimit(30);
    ipText.setFont(font);
    ipText.setFillColor(sf::Color::Blue);

    myTheme = simplgui::Theme::defaultTheme();
    auto resGetter = simplgui::FileResourcesGetter::create();
    button = simplgui::Button::create(resGetter);
    button = simplgui::Button::create(resGetter);
    button->setLabel(Button);

    button->setPosition(sf::Vector2f(getSize().x / 2 - (button->getLabel()).size() * 9, getSize().y / 2));
    button->setSize(sf::Vector2f((button->getLabel()).size() * 18, 48));
    button->setTheme(myTheme);
    server = std::make_shared<Server>(15001);

    button->onClicked.bind([&](simplgui::Button::Ptr button)
                           {

        if(!is_running)
        {
        try
        {   
            server->start_listen();
            is_running=server->is_started();
            std::cout<<"Server started\n";
        }
        catch (std::exception& e)
        {
            std::cout<<"Exception during server start\n";
            is_running=false;
            errorText.setString(e.what());
            errorText.setFillColor(sf::Color::Red);
            errorText.setPosition(getSize().x/2-errorText.getGlobalBounds().width/2,getSize().y/2+100);
            resultText.setFillColor(sf::Color::Red);
        }

        if(is_running)
        {
            std::cout<<"Post server start\n";
            resultText.setString("CONNECTED");
            resultText.setFillColor(sf::Color::Green);
            button->setLabel(U"Disconnect");
            resultText.setPosition(getSize().x/2-resultText.getGlobalBounds().width/2,getSize().y/2-50);
            button->setPosition(sf::Vector2f(getSize().x/2-(button->getLabel()).size()*8,getSize().y/2));
            button->setSize(sf::Vector2f((button->getLabel()).size()*16,48));
            ipText.setString("server->is_started()");
            ipText.setPosition(getSize().x/2-ipText.getGlobalBounds().width/2,getSize().y/2-80);
        }
        else
        {
             std::cout<<"Post server DISCONNECT\n";
            resultText.setString("DISCONNECTED");
            resultText.setFillColor(sf::Color::Red);
            resultText.setPosition(getSize().x/2-resultText.getGlobalBounds().width/2,getSize().y/2-50);
            button->setPosition(sf::Vector2f(getSize().x/2-(button->getLabel()).size()*8,getSize().y/2));
            button->setSize(sf::Vector2f((button->getLabel()).size()*16,48));
            ipText.setString("");

        }
        }
        else if(is_running)
        {
            std::cout<<"Stop server\n";
            server->stop_server();
            is_running=false;
            resultText.setString("DISCONNECTED");
            resultText.setFillColor(sf::Color::Red);
            button->setLabel(U"Connect");
            resultText.setPosition(getSize().x/2-resultText.getGlobalBounds().width/2,getSize().y/2-50);
            button->setPosition(sf::Vector2f(getSize().x/2-(button->getLabel()).size()*8,getSize().y/2));
            button->setSize(sf::Vector2f((button->getLabel()).size()*16,48));
            ipText.setString("");
            errorText.setString("");
        } });
}
void ServerGui::loop()
{
    while (isOpen())
    {
        clear(sf::Color::White);
        sf::Event event;
        auto dt = clock.restart();
        while (pollEvent(event))
        {
            button->update(dt);
            if (event.type == sf::Event::Closed)
            {
                server->stop_server();
                close();
            }
            button->processEvent(simplgui::Event(event, *this));
        }

        if (!is_running)
            draw(errorText);
        draw(resultText);
        draw(*button);
        draw(ipText);
        display();
    }
}

ServerGui::~ServerGui()
{
}

#ifndef CONNECTSCREEN_HPP
#define CONNECTSCREEN_HPP

#include "Screen.hpp"

#include "Button.hpp"
#include "Event.hpp"
#include "FileResourcesGetter.hpp"
#include "TextBox.hpp"
#include "Tools.hpp"
#include "Client.hpp"

class ConnectScreen:public Screen
    {
    /* komment commithoz */
    public:
        ConnectScreen(sf::RenderWindow *a);
        void draw();
        void handle(sf::Event& event);
        Client* getClient();
        virtual ~ConnectScreen();
    private:
        std::mutex my_mutex;
        bool isconnecting;
        std::string getName();
        std::string getIP();
        Client* testClient;
        sf::Text* resultText;
        sf::Font font;
        sf::Clock clock;
        sf::RenderWindow* app;
        simplgui::Button::Ptr button;
        simplgui::TextBox::Ptr textbox;
        simplgui::TextBox::Ptr name;
        simplgui::Theme myTheme;
        sf::Texture forBackground;
        sf::Sprite background;
        sf::Texture forCursor;
        sf::Sprite cursor;
};

#endif // CONNECTSCREEN_H

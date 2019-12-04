#ifndef CONNECTSCREEN_H
#define CONNECTSCREEN_H

#include "Screen.hpp"

#include "simplgui/Button.h"
#include "simplgui/Event.h"
#include "simplgui/FileResourcesGetter.h"
#include "simplgui/TextBox.h"
#include "simplgui/Tools.h"
#include "Client.hpp"

class ConnectScreen:public Screen
    {
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

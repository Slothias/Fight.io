#ifndef SERVERGUI_HPP
#define SERVERGUI_HPP


#include "Server.hpp"
#include <SFML/Graphics.hpp>
#include "simplgui/Button.h"
#include "simplgui/Event.h"
#include "simplgui/FileResourcesGetter.h"
#include "simplgui/TextBox.h"
#include "simplgui/Tools.h"

class ServerGui: public sf::RenderWindow
{
    public:
        ServerGui();
        void loop();
        ~ServerGui();
    private:
        sf::Clock clock;
        bool is_running;
        sf::Font font;
        sf::Text resultText;
        sf::Text errorText;
        sf::Text ipText;
        simplgui::Theme myTheme;
        simplgui::Button::Ptr button;
        Server* server;

};

#endif // SERVERGUI_HPP


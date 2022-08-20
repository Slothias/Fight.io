#ifndef SERVERGUI
#define SERVERGUI


#include "Server.hpp"
#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "Event.hpp"
#include "TextBox.hpp"
#include "Tools.hpp"

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
        simplgui::Button::Ptr connect_button;
        std::shared_ptr<Server> server;

};

#endif /* SERVERGUI */


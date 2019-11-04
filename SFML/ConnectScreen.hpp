#ifndef CONNECTSCREEN_H
#define CONNECTSCREEN_H

#include "Screen.hpp"

#include "simplgui/Button.h"
#include "simplgui/Event.h"
#include "simplgui/FileResourcesGetter.h"
#include "simplgui/TextBox.h"
#include "simplgui/Tools.h"

class ConnectScreen:public Screen
    {
    public:
        ConnectScreen(sf::RenderWindow *a);
        void draw();
        void handle(sf::Event& event);
        virtual ~ConnectScreen();
    private:
        sf::Clock clock;
        sf::RenderWindow* app;
        simplgui::Button::Ptr button;
        simplgui::TextBox::Ptr textbox;
        simplgui::Theme myTheme;
};

#endif // CONNECTSCREEN_H

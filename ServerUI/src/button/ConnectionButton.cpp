#include "button/ConnectionButton.hpp"
#include <iostream>
/// STATIC INITIALIZATION
std::u32string ConnectionButton::ButtonText::CONNECT = U"Connect";
std::u32string ConnectionButton::ButtonText::DISCONNECT = U"Disconnect";

ConnectionButton::ConnectionButton(simplgui::Theme& theme)
{
    setResourcesGetter(simplgui::FileResourcesGetter::create());
    setLabel(ConnectionButton::ButtonText::CONNECT);
    setTheme(theme);
    setLabel(ButtonText::CONNECT);
}

void ConnectionButton::onClick(std::function<void()> func )
{
    std::cout<<"micsoda\n";
    onClicked.bind([&](simplgui::Button::Ptr){
       func();
    });
}


ConnectionButton::~ConnectionButton()
{
}
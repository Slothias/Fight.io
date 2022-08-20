#ifndef CONNECTIONBUTTON
#define CONNECTIONBUTTON

#include "Button.hpp"
#include "FileResourcesGetter.hpp"

class ConnectionButton : public simplgui::Button
{
private:
    class ButtonText
    {
    public:
        static std::u32string CONNECT;
        static std::u32string DISCONNECT;
    };

public:
    ConnectionButton(simplgui::Theme& theme);
    void onClick(std::function<void()>);
    ~ConnectionButton();
};

#endif /* CONNECTIONBUTTON */

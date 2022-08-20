#ifndef SERVERWINDOW
#define SERVERWINDOW

#include "ServerEngine.hpp"
#include "ServerUI.hpp"

class ServerWindow
{
private:
    std::unique_ptr<ServerUI> serverUi;
    std::unique_ptr<ServerEngine> serverEngine;
public:
    ServerWindow();
    ~ServerWindow();
};

#endif /* SERVERWINDOW */

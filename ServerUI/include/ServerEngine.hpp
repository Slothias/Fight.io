#ifndef SERVERENGINE
#define SERVERENGINE


#include "Server.hpp"

class ServerEngine
{
private:
    std::unique_ptr<Server> serverPtr;
public:
    ServerEngine(/* args */);
    bool is_started() const;
    ~ServerEngine();
};


#endif /* SERVERENGINE */

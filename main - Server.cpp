#include "Server.hpp"

int main()
{

    std::thread t(&Server::runServer, new Server());
    t.join();
    return 0;
}

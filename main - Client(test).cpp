#include "Client.hpp"
int main()
{
    Client c("127.0.0.1",10043);
    std::thread t(&Client::runclient,&c);
    t.join();
    return 0;
}

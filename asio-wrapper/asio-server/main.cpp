#include "Server.hpp"
#include <vector>

int main() {
    Server srv(15001);
    srv.set_on_message_callback([](const std::string& message ){
        std::cout<<"Message: "<<message<<"\n";
    });
    srv.start_listen();
    std::cin.get();
    return 0;
}
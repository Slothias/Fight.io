//
// Created by sloy on 4/5/22.
//

#include "Client.hpp"

int main() {
    Client client("127.0.0.1",15001);
    client.set_on_message_callback([](const std::string& message){
        std::cerr<<"Message from server: "<<message<<"\n";
    });
    client.set_on_error_callback([](){
       std::cerr<<"Error happened! Exiting...!\n";
    });
    client.do_connect();
    return 0;
}
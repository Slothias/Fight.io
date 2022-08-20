#include "Server.hpp"

int main()
{
    Server srv(15001);
    srv.set_on_join_callback([&srv](){

        std::cout<<"join"<<std::endl;
        std::unique_ptr<Message> pistiMsg =  std::make_unique<Message>("Pistike!");
        srv.sendMessage(pistiMsg);
    });

    srv.set_on_message_callback([&srv](std::string msg){
        std::cout<<"message from client: "<<msg<<"\n";
        std::unique_ptr<Message> msgPTR = std::make_unique<Message>("Message from client: "+ msg);
        srv.sendMessage(msgPTR);
    });

    srv.start_listen();

    while (true)
    {
        
    }
    
}
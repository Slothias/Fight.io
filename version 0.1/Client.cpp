#include "Client.hpp"

Client::Client(const char* host,u_short port, Drawable_Player* _thisPlayer)
{
    is_running=startup(host,port);
    thisPlayer = _thisPlayer;
}
bool Client::startup(const char* host, u_short port)
{
      WSADATA WSAData; ///The WSADATA structure contains information about the Windows Sockets implementation.
    SOCKADDR_IN saddr; ///The sockaddr structure varies depending on the protocol selected. Except for the sin*_family parameter, sockaddr contents are expressed in network byte order.


  int iResult;
    iResult=WSAStartup(MAKEWORD(2, 2), &WSAData);
    if(iResult!=0) {
        std::cout<<"WSAStartup failed with error!"<<std::endl;
        return false;
    }
    iResult=server = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);///The AF_INET address family is the address family for IPv4. socket(address_family,type(in_this_case full duplex),protocol(TCP))
    if(iResult==INVALID_SOCKET) {
        std::cout<<"Socket failed with error"<<std::endl;
        WSACleanup();
        return false;
    }
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_family = AF_INET; ///IPv4 protocol
    addr.sin_port = htons(port); ///The htons function converts a u_short from host to TCP/IP network byte order (which is big-endian).
    iResult=connect(server, (SOCKADDR*)&addr, sizeof(addr));
    if(iResult==SOCKET_ERROR)
    {
        std::cout<<"Unable connect to server"<<std::endl;
        closesocket(server);
        WSACleanup();
        return false;
    }
    return true;

}

void Client::setconnected(bool c)
{
    my_mutex.lock();
    is_running=c;
    my_mutex.unlock();
}
bool Client::getconnected()
{
   my_mutex.lock();
   bool result = is_running;
   my_mutex.unlock();
   return result;
}
void Client::sendData(std::string data) {
if(getconnected())
{
    if (data.length() <= BUFFER_SIZE) {
        char buffer[BUFFER_SIZE];
        ZeroMemory(&buffer,sizeof(buffer));
        for (int i = 0; i < data.length(); i++)
            buffer[i] = data[i];
        send(server, buffer, sizeof(buffer), 0);
      //  std::cout<<"Message sent: "<<data<<std::endl;
    }
    }
}

std::string Client::getData() {
if(getconnected())
{
    char buffer[BUFFER_SIZE];
    ZeroMemory(&buffer,sizeof(buffer));
    recv(server,buffer, sizeof(buffer),0);
    return std::string(buffer);
}
    else
        return "closed connection";
}

void Client::closeConnection()
{
    Client::sendData("EXIT");
    shutdown(server,2);
    setconnected(false);
}

void Client::runclient()
{
    if(getconnected())
    {
        std::cout<<"CONNECTED"<<std::endl;
        std::thread get([this]()
                        {
                            while(getconnected())
                            {
                            std::string g = getData();
                            //std::cout<<"NEW MESSAGE"<<std::endl;
                            std::thread t (&Drawable_Player::update,&(*thisPlayer),g);
                            t.detach();
                            }
                        });
        std::string oldStatus;
        while(getconnected())
        {
               std::string this_status =thisPlayer->getMSG();
               if(oldStatus!=this_status)
               {
                   sendData(this_status);
                   oldStatus=this_status;
               }
               std::this_thread::sleep_for(std::chrono::milliseconds(12));
        }
        get.join();
    }
}

Client::~Client()
{
    closesocket(server);
    WSACleanup();
    std::cout<<"Socket closed"<<std::endl;
    delete thisPlayer;
}

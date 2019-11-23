#include "Client.hpp"

Client::Client()
{
    thisPlayer=nullptr;
}
std::string Client::tryToConnect(const char* host,u_short port,std::string name)
{
    is_running=startup(host,port);
    std::string g;
    if(is_running)
        {
        sendData(name);
        g= getData();
        }
    if(g=="OK")
        n=name;
    return g;

}
std::string Client::getName()
{
    return n;
}
void Client::addPlayer(player* p)
{
    my_mutex.lock();
    thisPlayer=p;
    my_mutex.unlock();
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
    sendData("EXIT");
    setconnected(false);
    shutdown(server,2);
}
void Client::notify()
{
    cv.notify_all();
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
                            if(g.find("Server")!=std::string::npos && g.find("EXIT")!=std::string::npos)
                                {
                                    setconnected(false);
                                    cv.notify_all();
                                }
                            std::thread t (&player::update,&(*thisPlayer),g);
                            t.detach();
                            }
                        });
        std::string oldstatus;
        std::unique_lock<std::mutex>  lck(thisMutex);
        while(getconnected())
        {

            while(!thisPlayer->getChange() && getconnected())
                cv.wait(lck);
            std::string this_status =thisPlayer->toString();
            sendData(this_status);
            thisPlayer->setChange(false);
            /*std::string thisstatus =thisPlayer->getMSG();
            if(thisstatus!=oldstatus)
            {
                sendData(thisstatus);
                oldstatus=thisstatus;
           }*/
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

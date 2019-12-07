#include "Client.hpp"

Client::Client()
{
    thisPlayer=nullptr;
    is_running=false;
    myClock = std::chrono::high_resolution_clock::now();
}
std::string Client::tryToConnect(const char* host,u_short port,std::string name)
{
    std::string result =startup(host,port);
    std::string g;
    if(is_running)
        {
        sendData(name);
        g= getData();
        }
    if(g=="OK")
        n=name;
    if(result==g)
        return g;
    else if(result=="OK")
        return g;
    else
        return result;

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
std::string Client::startup(const char* host, u_short port)
{
    is_running = true;
      WSADATA WSAData; ///The WSADATA structure contains information about the Windows Sockets implementation.
    SOCKADDR_IN saddr; ///The sockaddr structure varies depending on the protocol selected. Except for the sin*_family parameter, sockaddr contents are expressed in network byte order.


    int iResult;
    iResult=WSAStartup(MAKEWORD(2, 2), &WSAData);
    if(iResult!=0) {
        is_running=false;
        return "WSAStartup failed with error!";
    }
    iResult=server = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);///The AF_INET address family is the address family for IPv4. socket(address_family,type(in_this_case full duplex),protocol(TCP))
    if(iResult==INVALID_SOCKET) {
        WSACleanup();
        is_running=false;
        return "Socket failed with error";
    }
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_family = AF_INET; ///IPv4 protocol
    addr.sin_port = htons(port); ///The htons function converts a u_short from host to TCP/IP network byte order (which is big-endian).
    iResult=connect(server, (SOCKADDR*)&addr, sizeof(addr));
    if(iResult==SOCKET_ERROR)
    {
        closesocket(server);
        WSACleanup();
        is_running=false;
        return "Unable connect to server";
    }
    return "OK";

}

void Client::setconnected(bool c)
{
    my_mutex.lock();
    is_running=c;
    my_mutex.unlock();
}
bool Client::getconnected()
{
   //my_mutex.lock();
   bool result = is_running;
   //my_mutex.unlock();
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
    //std::cout<<"GETTING DATA...."<<std::endl;
    recv(server,buffer, sizeof(buffer),0);
    //std::cout<<"OK"<<std::endl;
    std::string result(buffer);
    return result;
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
        std::string oldstatus;
        std::unique_lock<std::mutex>  lck(thisMutex);
        long unsigned int run = 0;
         std::thread get([this,&run]()
                            {
                             while(getconnected())
                             {
                            std::string g = getData();
                            if((g.find("Server")!=std::string::npos && g.find("EXIT")!=std::string::npos)|| g.size()==0)
                                {
                                    setconnected(false);
                                    cv.notify_all();
                                }
                            std::thread t (&player::update,&(*thisPlayer),g);
                            t.detach();

                             }
                            });
        while(getconnected())
        {
            while(!thisPlayer->getChange() )
                cv.wait(lck);
            //auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( curTime - lastPoke ).count();
            if(thisPlayer->getRespawn())
            {
                sendData("RESPAWN");
                thisPlayer->setChange(false);
            }
            else
            {
            auto curTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( curTime - myClock ).count();
            if(duration>5 || thisPlayer->getPoke()){
                std::string this_status = thisPlayer->toString();
                sendData(this_status);
                thisPlayer->setChange(false);
                myClock = std::chrono::high_resolution_clock::now();
            }
            }
          /*

            get.join();*/
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

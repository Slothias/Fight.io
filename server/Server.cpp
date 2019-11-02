#include "Server.hpp"
#include <algorithm>
#include <string>
#include<future>
Server::Server()
{
    is_running=startup();
    getIP();
}

bool Server::startup()
{

    WSADATA WSAData; ///The WSADATA structure contains information about the Windows Sockets implementation.
    SOCKADDR_IN saddr; ///The sockaddr structure varies depending on the protocol selected. Except for the sin*_family parameter, sockaddr contents are expressed in network byte order.


    int iResult;
    iResult=WSAStartup(MAKEWORD(2, 2), &WSAData);
    if(iResult!=0)
    {
        std::cout<<"WSAStartup failed with error!"<<std::endl;
        return false;
    }
    iResult=server = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);///The AF_INET address family is the address family for IPv4. socket(address_family,type(in_this_case full duplex),protocol(TCP))
    if(iResult==INVALID_SOCKET)
    {
        std::cout<<"Socket failed with error"<<std::endl;
        WSACleanup();
        return false;
    }
    saddr.sin_addr.s_addr = INADDR_ANY; ///INADDR_ANY binds the socket to all available interfaces.
    saddr.sin_family = AF_INET; ///IPv4 protocol
    saddr.sin_port = htons(PORT); ///The htons function converts a u_short from host to TCP/IP network byte order (which is big-endian).
    iResult=bind(server, (SOCKADDR *) &saddr, sizeof(saddr));
    if(iResult==SOCKET_ERROR)
    {
        std::cout<<"bind failed with error"<<std::endl;

        shutdown(server,2);
        closesocket(server);
        WSACleanup();
        return false;
    }
    iResult=listen(server, 0);
    if(iResult==SOCKET_ERROR)
    {
        std::cout << "listen failed with error" << std::endl;

        shutdown(server,2);
        closesocket(server);
        WSACleanup();
        return false;
    }
    return true;
}
void Server::showStatus()
{
    std::cout<<myIP<<":"<<PORT<<std::endl;
}
void Server::getIP()
{
    system("ipconfig > ip.txt");
    std::ifstream myfile("ip.txt");
    std::string line;
    while(std::getline(myfile,line))
        if(line.find("IPv4")!=std::string::npos)
            myIP=line;
    int index=myIP.find(":")+2;
    myIP=myIP.substr(index, myIP.length()-index);
}
void Server::setconnected(bool c)
{
    my_mutex.lock();
    is_running=c;
    my_mutex.unlock();
}

bool Server::getconnected()
{
    my_mutex.lock();
    bool result=is_running;
    my_mutex.unlock();
    return result;
}

void Server::runServer()
{
    if(getconnected())
    {
        std::cout << "Listening for incoming connections..." << std::endl;
        showStatus();
        int clientsize = sizeof(clientAddr);
     /*   std::thread tick([this]()
        {
            while(getconnected())
            {
                if(current_msg.size()>0)
                {
                    std::pair<std::string, std::string> current;
                    my_mutex.lock();
                    current=current_msg.front();
                    current_msg.pop();
                    my_mutex.unlock();
                    sendData(current.first, current.second);
                    std::cout<<current_msg.size()<<std::endl;

                }
            }
        });*/
        while (getconnected())
        {
            SOCKET c = accept(server, (SOCKADDR *) &clientAddr, &clientsize);
            if (c != INVALID_SOCKET && getSize()<MAX_PLAYERS)
            {
                ServerAssistant* player=new ServerAssistant(c,this,"CLIENT "+std::to_string(getSize()));
                std::thread t(&ServerAssistant::run,&(*player));
                t.detach();
                my_mutex.lock();
                players.push_back(player);
                my_mutex.unlock();
            }
        }
       // tick.join();
    }
}
void Server::pushData(std::string data,std::string who)
{
    if(players.size()>1)
    {
    my_mutex.lock();
    current_msg.push(std::pair<std::string, std::string>(data,who));
    my_mutex.unlock();
    }
}
void Server::sendData(std::string data, std::string except)
{

    my_mutex.lock();
    for(ServerAssistant* s:players)
            if(s->getName()!=except)
                {
                    s->sendData(except+": "+data);
                   // std::cout<<"send: "<<s->getName()<<" except: "<<except<< "MSG: "<<data<<std::endl;
                }

    my_mutex.unlock();

}
int Server::getSize()
{
    int result=-1;
    my_mutex.lock();
    result=players.size();
    my_mutex.unlock();
    return result;
}

Server::ServerAssistant::ServerAssistant(SOCKET c,Server* m,std::string n)
{
    client = c;
    connected = true;
    me=m;
    name=n;
}
std::string Server::ServerAssistant::getName()
{
    return name;
}

void Server::ServerAssistant::setcon(bool c)
{
    my_mutex.lock();
    connected=c;
    my_mutex.unlock();
}
bool Server::ServerAssistant::getcon()
{
    bool result;
    my_mutex.lock();
    result=connected;
    my_mutex.unlock();
    return result;
}

void Server::ServerAssistant::sendData(std::string data)
{
    if(getcon())
    {
        if (data.length() <= BUFFER_SIZE)
        {
            my_mutex.lock();
            char buffer[BUFFER_SIZE];
            ZeroMemory(&buffer,sizeof(buffer));
            for (int i = 0; i < data.length(); i++)
                buffer[i] = data[i];
            send(client, buffer, sizeof(buffer), 0);
            my_mutex.unlock();
        }
    }
}

std::string Server::ServerAssistant::getData()
{
    if(getcon())
    {
        char buffer[BUFFER_SIZE];
        ZeroMemory(&buffer,sizeof(buffer));
        recv(client,buffer, sizeof(buffer),0);
        return std::string(buffer);

    }
}

void Server::ServerAssistant::deleteme()
{
    me->my_mutex.lock();
    me->players.erase(std::remove(me->players.begin(),me->players.end(),this),me->players.end());
    me->my_mutex.unlock();
    std::cout<<"lefutottam"<<std::endl;

}
void Server::ServerAssistant::closeConnection()
{
    setcon(false);
    closesocket(client);
    deleteme();

}
void Server::ServerAssistant::run()
{
    std::cout<<name<<" connected"<<std::endl;
    sendData("Your Name:"+ name);
    while(getcon())
    {
      //  std::this_thread::sleep_for(std::chrono::milliseconds(64));
        std::string msg=getData();
        if(msg.length()>0)
        {

            me->sendData(msg,name);
            if(msg.find("EXIT")!= std::string::npos)
                closeConnection();
        }
    }
    delete this;

}

Server::ServerAssistant::~ServerAssistant()
{
    std::cout<<"MEGHALTAM"<<std::endl;
    std::cout<<"Socket closed"<<std::endl;

}

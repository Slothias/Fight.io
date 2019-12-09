#include "Server.hpp"
#include <algorithm>
#include <string>
#include<sstream>
#include<future>
Server::Server()
{
}

bool Server::startup()
{

    WSADATA WSAData; ///The WSADATA structure contains information about the Windows Sockets implementation.
    SOCKADDR_IN saddr; ///The sockaddr structure varies depending on the protocol selected. Except for the sin*_family parameter, sockaddr contents are expressed in network byte order.


    int iResult;
    iResult=WSAStartup(MAKEWORD(2, 2), &WSAData);
    if(iResult!=0)
    {
        throw WSAStartupException();
        return false;
    }
    iResult=server = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);///The AF_INET address family is the address family for IPv4. socket(address_family,type(in_this_case full duplex),protocol(TCP))
    if(iResult==INVALID_SOCKET)
    {
        throw SocketException();
        WSACleanup();
        return false;
    }
    saddr.sin_addr.s_addr = INADDR_ANY; ///INADDR_ANY binds the socket to all available interfaces.
    saddr.sin_family = AF_INET; ///IPv4 protocol
    saddr.sin_port = htons(PORT); ///The htons function converts a u_short from host to TCP/IP network byte order (which is big-endian).
    iResult=bind(server, (SOCKADDR *) &saddr, sizeof(saddr));
    if(iResult==SOCKET_ERROR)
    {

        shutdown(server,2);
        closesocket(server);
        WSACleanup();
        throw BindException();
        return false;
    }
    iResult=listen(server, 0);
    if(iResult==SOCKET_ERROR)
    {
        throw ListenException();

        shutdown(server,2);
        closesocket(server);
        WSACleanup();
        return false;
    }
    return true;
}
void Server::tryToConnect()
{
    getIP();
    is_running = startup();
    myEngine = myEngine->GetInstance(this);
    maxplayers= myEngine->GetMaxPlayers();
}
std::string Server::showStatus()
{
    std::stringstream ss;
    ss<<myIP<<":"<<PORT<<std::endl;
    return ss.str();
}
void Server::getIP()
{
    myIP="127.0.0.1";
    system("ipconfig > ip.txt");
    std::ifstream myfile("ip.txt");
    std::string line;
    while(std::getline(myfile,line))
        if(line.find("IPv4")!=std::string::npos)
            myIP=line;
    if(myIP!="127.0.0.1")
    {
        int index=myIP.find(":")+2;
        myIP=myIP.substr(index, myIP.length()-index);
    }
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
        int clientsize = sizeof(clientAddr);
        while (getconnected())
        {
            SOCKET c = accept(server, (SOCKADDR *) &clientAddr, &clientsize);
            if (c != INVALID_SOCKET && getSize()<maxplayers)
            {
                ServerAssistant* player=new ServerAssistant(c,this,"");
                std::string g = player->getData();
                std::string result = myEngine->CreatePlayer(g);
                player->sendData(result);
                if(result == "OK")
                {
                    std::cout << "OOKK" << std::endl;
                    player->setName(g);
                    std::thread t(&ServerAssistant::run,&(*player));
                    t.detach();
                    my_mutex.lock();
                    players.push_back(player);
                    my_mutex.unlock();
                }
                else
                {
                    delete player;
                }
            }
        }
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
void Server::sendData(std::string data)
{

    my_mutex.lock();
    for(ServerAssistant* s:players)
        s->sendData(data);
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
void Server::closeServer()
{
    is_running = false;
    sendData("Server:EXIT");
    for(ServerAssistant* s : players)
        s->setcon(false);
    while(getSize()>0)
        players.pop_back();
    shutdown(server,2);
    closesocket(server);
    WSACleanup();
}
Server::~Server()
{
    delete myEngine;

}
Server::ServerAssistant::ServerAssistant(SOCKET c,Server* m,std::string n)
{
    client = c;
    connected = true;
    me=m;
    name=n;
}
void Server::ServerAssistant::setName(std::string n)
{
    my_mutex.lock();
    name = n;
    my_mutex.unlock();
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

}
void Server::ServerAssistant::closeConnection()
{
    setcon(false);
    closesocket(client);
    deleteme();

}
void Server::ServerAssistant::run()
{

    std::string o = me->myEngine->GetMe(name);
    std::cout<<o<<std::endl;
    me->sendData(o);
    for(std::string s: me->myEngine->getState(name))
    {
        sendData(s);
    }
    while(getcon())
    {
        std::string msg=getData();
        if(msg.length()>0)
        {
            if(msg.find("RESPAWN")!=std::string::npos)
            {
                std::cout<<"Res: "<<name<<std::endl;
                me->sendData(me->myEngine->ReSpawn(name));
            }
            else
            {
            for(std::string s : me->myEngine->CheckRequest(name, msg))
            {
                    me->sendData(s);
            }
            }
            if(msg.find("EXIT")!= std::string::npos)
            {
                closeConnection();
            }
        }else{
            closeConnection();
            me->myEngine->CheckRequest(name,"EXIT");
        }
    }
    delete this;

}

Server::ServerAssistant::~ServerAssistant()
{
}

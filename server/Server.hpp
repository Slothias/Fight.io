#ifndef SERVER_H
#define SERVER_H
#include <thread>
#include <winsock2.h>
#include<iostream>
#include<atomic>
#include<vector>
#include<mutex>
#include<queue>
#include<utility>
#include<fstream>
#include "ServerException.h"
#include "GameEngine.hpp"
class Server
{
private:
    class ServerAssistant
    {
    private:
        std::string name;
        Server* me;
        static const int BUFFER_SIZE=1024;
        std::mutex my_mutex;
        int position;
        SOCKET client;
        bool connected;

    public:
        std::string getName();
        ServerAssistant(SOCKET c,Server* me, std::string name);
        void run();
        void deleteme();
        void setcon(bool c);
        bool getcon();
        void closeConnection();
        void sendData(std::string data);
        std::string getData();
        void pushData();
        ~ServerAssistant();
    };
    GameEngine* myEngine;
    std::queue<std::pair<std::string,std::string>> current_msg;
    SOCKADDR_IN clientAddr;
    SOCKET server;
    std::string myIP;
    void getIP();
    bool is_running;
    std::mutex my_mutex;
    bool startup();
    const u_short PORT=10043;
    static const int MAX_PLAYERS=10;
    int getSize();
    std::vector<ServerAssistant*> players;
    void sendData(std::string data,std::string except); ///send to all
    void pushData(std::string data, std::string who);
public:
    Server();
    void tryToConnect();
    void  runServer();
    bool getconnected();
    void setconnected(bool c);
    std::string showStatus();
    void closeServer();
    ~Server();

};

#endif // SERVER_H

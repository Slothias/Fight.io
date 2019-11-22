#ifndef CLIENT_H
#define CLIENT_H
#include<thread>
#include<iostream>
#include <winsock2.h>
#include<mutex>
#include "player.hpp"
#include<condition_variable>

class Client
{
public:
    Client(const char* host,u_short port, player* _thisPlayer);
    void runclient();
    bool getconnected();
    void notify();
    ~Client();
    void closeConnection();
private:
    std::condition_variable cv;
    std::mutex thisMutex;
    bool is_running;
    std::mutex my_mutex;
    SOCKET server;
    SOCKADDR_IN addr;
    bool startup(const char* host, u_short port);
    void setconnected(bool c);
    void sendData(std::string msg);
    std::string getData();
    static const int BUFFER_SIZE=1024;
    player* thisPlayer;
};

#endif // CLIENT_H

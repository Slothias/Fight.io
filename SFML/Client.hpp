#ifndef CLIENT_H
#define CLIENT_H
#include<thread>
#include<iostream>
#include <winsock2.h>
#include <mutex>
#include <chrono>
#include "../Includes/player.hpp"
#include<condition_variable>

class Client
{
public:
    Client();
    void runclient();
    bool getconnected();
    void notify();
    std::string getName();
    std::string tryToConnect(const char* host,u_short port,std::string name);
    void addPlayer(player* p);
    ~Client();
    void closeConnection();
private:
    std::string n;
    std::condition_variable cv;
    std::mutex thisMutex;
    bool is_running;
    std::mutex my_mutex;
    SOCKET server;
    SOCKADDR_IN addr;
    std::string startup(const char* host, u_short port);
    void setconnected(bool c);
    void sendData(std::string msg);
    std::string getData();
    static const int BUFFER_SIZE=1024;
    player* thisPlayer;
    std::chrono::high_resolution_clock::time_point myClock;
};

#endif // CLIENT_H

#ifndef CLIENT_H
#define CLIENT_H
#include<thread>
#include<iostream>
#include <winsock2.h>
#include<mutex>

class Client
{
public:
    Client(const char* host,u_short port);
    void runclient();
    ~Client();
private:
    bool is_running;
    std::mutex my_mutex;
    SOCKET server;
    SOCKADDR_IN addr;
    bool startup(const char* host, u_short port);
    bool getconnected();
    void setconnected(bool c);
    void sendData(std::string msg);
    std::string getData();
    static const int BUFFER_SIZE=1024;
};

#endif // CLIENT_H

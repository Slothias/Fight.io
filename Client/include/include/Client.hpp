#ifndef CLIENT_HPP
#define CLIENT_HPP
#include<thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <mutex>
#include <chrono>
#include "Player.hpp"
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
    void addPlayer(Player* p);
    void sendData(std::string msg);
    std::string getData();
    ~Client();
    void closeConnection();
private:
    std::string n;
    std::condition_variable cv;
    std::mutex thisMutex;
    bool is_running;
    std::mutex my_mutex;
    int server;
    int addr;
    std::string startup(const char* host, u_short port);
    void setconnected(bool c);
    static const int BUFFER_SIZE=1024;
    Player* thisPlayer;
    std::chrono::high_resolution_clock::time_point myClock;
};

#endif // CLIENT_H

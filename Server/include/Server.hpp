#ifndef SERVER_HPP
#define SERVER_HPP



#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <atomic>
#include <vector>
#include <mutex>
#include <queue>
#include <utility>
#include <fstream>
#include "ServerException.hpp"
#include "GameEngine.hpp"

using namespace boost::asio;

class GameEngine;

class Server {
private:
    class ServerAssistant {
    private:
        std::string name;
        Server *me;
        static const int BUFFER_SIZE = 1024;
        std::mutex my_mutex;
        int position;
        int client;
        bool connected;

    public:
        std::string getName();

        void setName(std::string n);

        ServerAssistant(int c, Server *me, std::string name);

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




    GameEngine *myEngine;
    std::queue<std::pair<std::string, std::string>> current_msg;
    int server;
    std::string myIP;

    void getIP();

    bool is_running;
    std::mutex my_mutex;

    bool startup();

    const u_short PORT = 10043;
    int maxplayers;

    int getSize();

    std::vector<ServerAssistant *> players;

    void pushData(std::string data, std::string who);

public:
    void sendData(std::string data); /// send to all
    Server();

    void tryToConnect();

    void runServer();

    bool getconnected();

    void setconnected(bool c);

    std::string showStatus();

    void closeServer();

    ~Server();
};

#endif // SERVER_H

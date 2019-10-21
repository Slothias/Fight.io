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
    std::queue<std::pair<std::string,std::string>> current_msg;
    SOCKADDR_IN clientAddr;
    SOCKET server;
    bool is_running;
    std::mutex my_mutex;
    bool startup();
    const u_short PORT=10043;
    static const int MAX_PLAYERS=10;
    int getSize();
    void setconnected(bool c);
    bool getconnected();
    std::vector<ServerAssistant*> players;
    void sendData(std::string data,std::string except); ///send to all
    void pushData(std::string data, std::string who);
public:
    Server();
    void  runServer();
    ~Server();

};

#endif // SERVER_H

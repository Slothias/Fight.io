#include "Server.hpp"
#include <algorithm>
#include <string>
#include<sstream>
#include<future>
#include <boost/bind.hpp>

Server::Server() {
}

bool Server::startup() {

    io_service t_io_service;
    m_acceptor = boost::make_shared<ip::tcp::acceptor>(t_io_service, ip::tcp::endpoint(ip::tcp::v4(), PORT));
    m_socket = boost::make_shared<boost::asio::ip::tcp::socket>(t_io_service);
    std::cout << m_socket->local_endpoint().address() << '\n';
    return true;
}

void Server::tryToConnect() {
    getIP();
    is_running = startup();
    myEngine = myEngine->GetInstance(this);
    maxplayers = myEngine->GetMaxPlayers();
}

std::string Server::showStatus() {
    std::stringstream ss;
    ss << myIP << ":" << PORT << std::endl;
    return ss.str();
}

void Server::getIP() {
    myIP = "127.0.0.1";
    system("ipconfig > ip.txt");
    std::ifstream myfile("ip.txt");
    std::string line;
    while (std::getline(myfile, line))
        if (line.find("IPv4") != std::string::npos)
            myIP = line;
    if (myIP != "127.0.0.1") {
        int index = myIP.find(":") + 2;
        myIP = myIP.substr(index, myIP.length() - index);
    }
}

void Server::setconnected(bool c) {
    my_mutex.lock();
    is_running = c;
    my_mutex.unlock();
}

bool Server::getconnected() {
    my_mutex.lock();
    bool result = is_running;
    my_mutex.unlock();
    return result;
}

void Server::runServer() {
    while (getconnected()) {
        m_acceptor->async_accept([this](std::error_code ec, ip::tcp::socket t_new_socket) {

        })
        if (getSize() < maxplayers) {
            auto *player = new ServerAssistant(c, this, "");
            std::string g = player->getData();
            std::cout << g << std::endl;
            std::string result = myEngine->CreatePlayer(g);
            player->sendData(result);
            if (result == "OK") {
                std::cout << "OOKK" << std::endl;
                player->setName(g);
                std::thread t(&ServerAssistant::run, &(*player));
                t.detach();
                my_mutex.lock();
                players.push_back(player);
                my_mutex.unlock();
            } else {
                delete player;
            }
        }
    }
}

void Server::pushData(std::string data, std::string who) {
    if (players.size() > 1) {
        my_mutex.lock();
        current_msg.push(std::pair<std::string, std::string>(data, who));
        my_mutex.unlock();
    }
}

void Server::sendData(std::string data) {

    my_mutex.lock();
    for (ServerAssistant *s: players)
        s->sendData(data);
    my_mutex.unlock();

}

int Server::getSize() {
    int result = -1;
    my_mutex.lock();
    result = players.size();
    my_mutex.unlock();
    return result;
}

void Server::closeServer() {
    is_running = false;
    sendData("Server:EXIT");
    for (ServerAssistant *s: players)
        s->setcon(false);
    while (getSize() > 0)
        players.pop_back();
    shutdown(server, 2);
}

Server::~Server() {
    delete myEngine;

}

Server::ServerAssistant::ServerAssistant(int c, Server *m, std::string n) {
    client = c;
    connected = true;
    me = m;
    name = n;
}

void Server::ServerAssistant::setName(std::string n) {
    my_mutex.lock();
    name = n;
    my_mutex.unlock();
}

std::string Server::ServerAssistant::getName() {
    return name;
}

void Server::ServerAssistant::setcon(bool c) {
    my_mutex.lock();
    connected = c;
    my_mutex.unlock();
}

bool Server::ServerAssistant::getcon() {
    bool result;
    my_mutex.lock();
    result = connected;
    my_mutex.unlock();
    return result;
}

void Server::ServerAssistant::sendData(std::string data) {
    if (getcon()) {
        if (data.length() <= BUFFER_SIZE) {
            my_mutex.lock();
            char buffer[BUFFER_SIZE];
            bzero(&buffer, sizeof(buffer));
            for (int i = 0; i < data.length(); i++)
                buffer[i] = data[i];
            send(client, buffer, sizeof(buffer), 0);
            my_mutex.unlock();
        }
    }
}

std::string Server::ServerAssistant::getData() {
    if (getcon()) {
        char buffer[BUFFER_SIZE];
        bzero(&buffer, sizeof(buffer));
        recv(client, buffer, sizeof(buffer), 0);
        return std::string(buffer);

    }
}

void Server::ServerAssistant::deleteme() {
    me->my_mutex.lock();
    me->players.erase(std::remove(me->players.begin(), me->players.end(), this), me->players.end());
    me->my_mutex.unlock();
    //std::cout<<"lefutottam"<<std::endl;

}

void Server::ServerAssistant::closeConnection() {
    setcon(false);
    deleteme();

}

void Server::ServerAssistant::run() {

    std::string o = me->myEngine->GetMe(name);
    std::cout << "send to all: " << o << std::endl;
    me->sendData(o);
    for (std::string s: me->myEngine->getState(name)) {
        sendData(s);
        std::cout << name << " --> " << s << std::endl;
    }
    while (getcon()) {
        std::string msg = getData();
        if (msg.length() > 0) {
            if (msg.find("RESPAWN") != std::string::npos) {
                std::cout << "Res: " << name << std::endl;
                me->sendData(me->myEngine->ReSpawn(name));
            } else {
                for (std::string s: me->myEngine->CheckRequest(name, msg)) {
                    me->sendData(s);
                }
            }
            if (msg.find("EXIT") != std::string::npos) {
                closeConnection();
            }
        } else {
            closeConnection();
            me->myEngine->CheckRequest(name, "EXIT");
        }
    }
    delete this;

}

Server::ServerAssistant::~ServerAssistant() {
}

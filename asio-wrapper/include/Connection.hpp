//
// Created by sloy on 2022. 04. 02..
//

#ifndef FIGHT_IO_CONNECTION_HPP
#define FIGHT_IO_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <istream>
#include <queue>
#include <Server.hpp>

class Server;

enum CONTROL_MSG {
    DISCONNECTED,
};

class Connection : public std::enable_shared_from_this<Connection> {
private:
    boost::asio::ip::tcp::socket socket;
    boost::asio::streambuf streambuf;
    std::vector<std::string> messages;
public:
    Connection(boost::asio::ip::tcp::socket &&socket);

    std::string get_client_address() const;

    std::vector<std::string> read_messages();

    void start();

    void disconnect();

    ~Connection();
};


#endif //FIGHT_IO_CONNECTION_HPP

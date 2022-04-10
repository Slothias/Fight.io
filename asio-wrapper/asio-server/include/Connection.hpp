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
#include <Message.hpp>
#include <utility>
#include <shared_mutex>
#include "ControlMessage.hpp"
class Server;


class Connection : public std::enable_shared_from_this<Connection> {
private:
    boost::asio::ip::tcp::socket socket;
    boost::asio::streambuf read_buf;
    boost::asio::streambuf write_buf;

    std::shared_mutex message_shared_mutex;
    std::vector<std::shared_ptr<Message>> messages;
public:
    Connection(boost::asio::ip::tcp::socket &&socket);
    std::vector<std::shared_ptr<Message>> read_messages();

    void start();
    void sendMessage(std::unique_ptr<Message>& message);

    void disconnect();

    ~Connection();
};


#endif //FIGHT_IO_CONNECTION_HPP

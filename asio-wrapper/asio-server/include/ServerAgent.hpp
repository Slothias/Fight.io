//
// Created by sloy on 2022. 04. 02..
//

#ifndef FIGHT_IO_SERVERAGENT_HPP
#define FIGHT_IO_SERVERAGENT_HPP

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
#include "NetworkCallback.hpp"
class Server;



class ServerAgent : public std::enable_shared_from_this<ServerAgent>, public NetworkCallback {
private:
    boost::asio::ip::tcp::socket socket;
    boost::asio::streambuf read_buf;
    void async_read();
public:
    ServerAgent(boost::asio::ip::tcp::socket &&socket, std::function<void(std::string)>&& t_message_handler, std::function<void()>&& t_error_handler);
    void start();
    void sendMessage(std::unique_ptr<Message>& message);

    void disconnect();

    ~ServerAgent();
};


#endif //FIGHT_IO_SERVERAGENT_HPP

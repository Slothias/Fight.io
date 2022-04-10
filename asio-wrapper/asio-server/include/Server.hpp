//
// Created by sloy on 2022. 04. 02..
//

#ifndef FIGHT_IO_SERVER_HPP
#define FIGHT_IO_SERVER_HPP

#include <optional>
#include "Connection.hpp"
#include <boost/make_shared.hpp>
#include <vector>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>

class Message;
class Connection;
class Server {

public:
    Server()=default;
    Server(std::uint16_t port);

    Server(boost::asio::io_context &io_context, std::uint16_t port);

    void async_accept();

    void disconnect_client(const std::shared_ptr<Connection> &client_ptr);

    std::vector<std::shared_ptr<Message>> read_all_messages();
    void sendMessage(std::unique_ptr<Message>& message);

    void stop_server();

    ~Server() = default;

private:
    boost::asio::io_context &io_context;
    boost::asio::ip::tcp::acceptor acceptor;
    std::optional<boost::asio::ip::tcp::socket> socket;
    std::vector<std::shared_ptr<Connection>> clients;
};


#endif //FIGHT_IO_SERVER_HPP

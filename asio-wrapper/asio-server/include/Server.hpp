//
// Created by sloy on 2022. 04. 02..
//

#ifndef FIGHT_IO_SERVER_HPP
#define FIGHT_IO_SERVER_HPP

#include "ServerAgent.hpp"
#include <vector>
#include "NetworkCallback.hpp"
#include "NetworkConnection.hpp"
class Message;
class ServerAgent;
class Server: public NetworkCallback, public NetworkConnection {

public:
    Server()=delete;
    Server(std::uint16_t port);

    void start_listen();

    void disconnect_client(const std::shared_ptr<ServerAgent> &client_ptr);

    void sendMessage(std::unique_ptr<Message>& message);


    void stop_server();

    bool is_started() const;

    ~Server() = default;

private:
    boost::asio::ip::tcp::acceptor acceptor;
    std::vector<std::shared_ptr<ServerAgent>> clients;
    void async_accept();
    bool started;
};


#endif //FIGHT_IO_SERVER_HPP

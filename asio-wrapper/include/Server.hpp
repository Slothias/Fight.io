//
// Created by sloy on 2022. 04. 02..
//

#ifndef FIGHT_IO_SERVER_HPP
#define FIGHT_IO_SERVER_HPP
#include <optional>
#include "Connection.hpp"
#include <boost/make_shared.hpp>
class Server {

public:
    Server(boost::asio::io_context& io_context, std::uint16_t port);
    void async_accept();

private:
    boost::asio::io_context& io_context;
    boost::asio::ip::tcp::acceptor acceptor;
    std::optional<boost::asio::ip::tcp::socket> socket;
};


#endif //FIGHT_IO_SERVER_HPP

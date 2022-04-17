//
// Created by sloy on 4/5/22.
//

#ifndef FIGHT_IO_CLIENT_HPP
#define FIGHT_IO_CLIENT_HPP

#include <boost/asio/io_context.hpp>
#include <optional>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include "NetworkCallback.hpp"
#include <boost/asio/connect.hpp>
#include <boost/asio/read_until.hpp>
#include<iostream>
class Client: public NetworkCallback{


private:
    boost::asio::io_context &io_context;
    boost::asio::ip::tcp::socket socket;
    boost::asio::ip::tcp::endpoint endpoint;
    boost::asio::streambuf read_buf;

    void async_read();

public:
    Client(const std::string& host, std::uint16_t port);
    void do_connect();

    ~Client() = default;

};


#endif //FIGHT_IO_CLIENT_HPP

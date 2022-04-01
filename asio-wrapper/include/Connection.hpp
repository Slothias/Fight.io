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
class Connection : public std::enable_shared_from_this<Connection>{
private:
    boost::asio::ip::tcp::socket socket;
    boost::asio::streambuf streambuf;

public:
    explicit Connection(boost::asio::ip::tcp::socket&& socket);
    ~Connection();
    void start() ;
};


#endif //FIGHT_IO_CONNECTION_HPP

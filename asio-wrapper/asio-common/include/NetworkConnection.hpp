//
// Created by sloy on 2022.04.18..
//

#ifndef FIGHT_IO_NETWORKCONNECTION_HPP
#define FIGHT_IO_NETWORKCONNECTION_HPP

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/read_until.hpp>

class NetworkConnection {
protected:
    boost::asio::io_context& m_io_context;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::streambuf m_read_buf;

public:
    NetworkConnection();
};


#endif //FIGHT_IO_NETWORKCONNECTION_HPP

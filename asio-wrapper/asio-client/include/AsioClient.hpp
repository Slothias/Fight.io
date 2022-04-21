//
// Created by sloy on 4/5/22.
//

#ifndef FIGHT_IO_ASIOCLIENT_HPP
#define FIGHT_IO_ASIOCLIENT_HPP


#include "NetworkCallback.hpp"
#include "NetworkConnection.hpp"
#include <boost/asio/write.hpp>
#include <iostream>
class AsioClient: public NetworkCallback, public NetworkConnection{


private:
    boost::asio::ip::tcp::endpoint endpoint;
    void async_read();

public:
    AsioClient(const std::string& host, std::uint16_t port);
    void do_connect();
    void send_message(std::string& message);

    ~AsioClient() = default;

};


#endif //FIGHT_IO_ASIOCLIENT_HPP

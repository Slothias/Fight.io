//
// Created by sloy on 2022.04.18..
//

#include "NetworkConnection.hpp"


NetworkConnection::NetworkConnection(): m_io_context(*new boost::asio::io_context),m_socket(m_io_context) {

}

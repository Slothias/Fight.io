#include "Server.hpp"

Server::Server(boost::asio::io_context &io_context, std::uint16_t port) : io_context(io_context),
                                                                          acceptor(io_context,
                                                                                   boost::asio::ip::tcp::endpoint(
                                                                                           boost::asio::ip::tcp::v4(),
                                                                                           port)) {}

void Server::async_accept() {
    socket.emplace(io_context);

    acceptor.async_accept(*socket, [&] (boost::system::error_code error)
    {
        std::cout<<"Connected!\n";
        std::make_shared<Connection>(std::move(*socket))->start();
        async_accept();
    });
}
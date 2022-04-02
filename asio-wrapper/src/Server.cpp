#include "Server.hpp"


#include <arpa/inet.h>


Server::Server(std::uint16_t port) : io_context(*new boost::asio::io_context), acceptor(io_context,
                                                                                        boost::asio::ip::tcp::endpoint(
                                                                                                boost::asio::ip::tcp::v4(),
                                                                                                port)) {
    async_accept();
    io_context.run();
}

Server::Server(boost::asio::io_context &io_context, std::uint16_t port) : io_context(io_context),
                                                                          acceptor(io_context,
                                                                                   boost::asio::ip::tcp::endpoint(
                                                                                           boost::asio::ip::tcp::v4(),
                                                                                           port)) {
    async_accept();
    io_context.run();
}

void Server::disconnect_client(const std::shared_ptr<Connection> &client_ptr) {
    clients.erase(std::remove(clients.begin(), clients.end(), client_ptr), clients.end());
    std::cout << "Server erased client: " << client_ptr->get_client_address() << ", number of clients: "
              << clients.size() << "\n";
    client_ptr->disconnect();
}


void Server::async_accept() {
    socket.emplace(io_context);

    acceptor.async_accept(*socket, [&](boost::system::error_code error) {
        std::shared_ptr<Connection> client = std::make_shared<Connection>(std::move(*socket));
        clients.push_bac k(client);
        client->start();
        async_accept();
    });
}

void Server::stop_server() {
    std::for_each(clients.begin(), clients.end(), [&](const auto &item) {
        disconnect_client(item);
    });
    io_context.stop();
}

Server::~Server() {
}

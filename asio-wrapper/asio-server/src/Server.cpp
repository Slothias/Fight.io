#include "Server.hpp"


#include <arpa/inet.h>


Server::Server(std::uint16_t port) : io_context(*new boost::asio::io_context), acceptor(io_context,
                                                                                        boost::asio::ip::tcp::endpoint(
                                                                                                boost::asio::ip::tcp::v4(),
                                                                                                port)) {
    async_accept();
    std::thread([&]() {
        io_context.run();
    }).detach();
    std::cout << "Server listening at port: " << port << "\n";
}

Server::Server(boost::asio::io_context &io_context, std::uint16_t port) : io_context(io_context),
                                                                          acceptor(io_context,
                                                                                   boost::asio::ip::tcp::endpoint(
                                                                                           boost::asio::ip::tcp::v4(),
                                                                                           port)) {
}

void Server::disconnect_client(const std::shared_ptr<Connection> &client_ptr) {
    clients.erase(std::remove(clients.begin(), clients.end(), client_ptr), clients.end());
    std::cout << "asio-client erased from server, connected clients: " << clients.size() << "\n";
}

std::vector<std::shared_ptr<Message>> Server::read_all_messages() {
    std::vector<std::shared_ptr<Message>> result;
    std::for_each(clients.begin(), clients.end(), [&result](const std::shared_ptr<Connection> &client) {
        if (client.get()) {
            std::vector<std::shared_ptr<Message>> client_messages = client->read_messages();
            std::for_each(client_messages.begin(), client_messages.end(),
                          [&result](const std::shared_ptr<Message> &message_ptr) {
                              result.push_back(message_ptr);
                          });
        }
    });
    std::sort(result.begin(), result.end(), [](std::shared_ptr<Message> &a, std::shared_ptr<Message> &b) {
        return a->get_timestamp() > b->get_timestamp();
    });
    return result;
}

void Server::async_accept() {
    socket.emplace(io_context);

    acceptor.async_accept(*socket, [&](boost::system::error_code error) {
        std::shared_ptr<Connection> client = std::make_shared<Connection>(std::move(*socket));
        clients.push_back(client);
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

void Server::sendMessage(std::unique_ptr<Message> &message) {
    if (message->get_cast() == Cast::BROADCAST) {
        std::for_each(clients.begin(), clients.end(), [&message](const std::shared_ptr<Connection> &client) {
            client->sendMessage(message);
        });
    } else {
        auto client_iterator = std::find_if(clients.begin(), clients.end(),
                                            [&message](const std::shared_ptr<Connection> &client) {
                                                return message->get_connection() == client;
                                            });
        if (client_iterator != clients.end()) {
            client_iterator->get()->sendMessage(message);
        }
    }
}

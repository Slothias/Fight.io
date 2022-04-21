#include "Server.hpp"

Server::Server(std::uint16_t port) : acceptor(m_io_context,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),port)) {
}


void Server::start_listen() {
    async_accept();
    std::thread([&]() {
        m_io_context.run();
    }).detach();
    std::cout << "Server listening..\n";
}

void Server::disconnect_client(const std::shared_ptr<ServerAgent> &client_ptr) {

    if(on_leave_callback){
        on_leave_callback();
    }
    clients.erase(std::remove(clients.begin(), clients.end(), client_ptr), clients.end());
    std::cout << "asio-client erased from server, connected clients: " << clients.size() << "\n";
}


void Server::async_accept() {

    acceptor.async_accept(m_socket, [&](boost::system::error_code error) {
        std::shared_ptr<ServerAgent> client = std::make_shared<ServerAgent>(std::move(m_socket), std::move(on_message_callback), std::move(on_error_callback));
        clients.push_back(client);
        if(on_join_callback){
            on_join_callback();
        }
        client->start();
        async_accept();
    });
}

void Server::stop_server() {
    std::for_each(clients.begin(), clients.end(), [&](const auto &item) {
        disconnect_client(item);
    });
    m_io_context.stop();
}

void Server::sendMessage(std::unique_ptr<Message> &message) {
    if (message->get_cast() == Cast::BROADCAST) {
        std::for_each(clients.begin(), clients.end(), [&message](const std::shared_ptr<ServerAgent> &client) {
            client->sendMessage(message);
        });
    } else {
        auto client_iterator = std::find_if(clients.begin(), clients.end(),
                                            [&message](const std::shared_ptr<ServerAgent> &client) {
                                                return message->get_connection() == client;
                                            });
        if (client_iterator != clients.end()) {
            client_iterator->get()->sendMessage(message);
        }
    }
}

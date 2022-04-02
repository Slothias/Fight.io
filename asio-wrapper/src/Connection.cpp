#include "Connection.hpp"

#include <utility>


Connection::Connection(boost::asio::ip::tcp::socket &&socket) : socket(
        std::move(socket)) {
    std::cout << "Client connected from host: " << get_client_address() << "\n";
}

void Connection::start() {

    std::cout << "Waiting for message from client: " << socket.remote_endpoint().address() << ":"
              << socket.remote_endpoint().port() << "\n";
    boost::asio::async_read_until(socket,
                                  streambuf,
                                  '\n',
                                  [self = shared_from_this()](boost::system::error_code error,
                                                              long bytes_transferred) {
                                      if (!error) {
                                          std::string command{
                                                  buffers_begin(self->streambuf.data()),
                                                  buffers_begin(self->streambuf.data()) + bytes_transferred
                                                  - std::string("\n").size()};

                                          self->streambuf.consume(bytes_transferred);
                                          self->messages.push_back(command);
                                      } else {
                                          std::cout << error << std::endl;
                                          self->messages.push_back(std::to_string(DISCONNECTED));
                                          self->disconnect();
                                      }
                                  });
}

Connection::~Connection() {
    std::cout << "Connection destruction" << "\n";
}

void Connection::disconnect() {
    if (socket.is_open()) {
        std::cout << "Client disconnected with ip: "
                  << socket.remote_endpoint().address() << " and port: "
                  << socket.remote_endpoint().port() << "\n";
        socket.close();
    }
}

std::vector<std::string> Connection::read_messages() {
    const std::vector<std::string> result = messages;
    messages.clear();
    return result;
}

std::string Connection::get_client_address() const {
    std::string remote_addr = socket.remote_endpoint().address().to_string();
    remote_addr.append(":") += std::to_string(socket.remote_endpoint().port());
    return remote_addr;
}
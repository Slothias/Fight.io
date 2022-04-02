#include "Connection.hpp"

#include <utility>


Connection::Connection(boost::asio::ip::tcp::socket &&socket,Server* server) : socket(
        std::move(socket)) {
    std::cout << "Client connected from host: " << get_client_address() << "\n";
    this->server =server;
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

                                          // Consume through the first delimiter so that subsequent async_read_until
                                          // will not reiterate over the same data.
                                          self->streambuf.consume(bytes_transferred);
                                          if (command.find("EXIT") == std::string::npos) {
                                              std::cout << "msg :" << command << '\n';
                                              self->start();
                                          } else {
                                              self->server->disconnect_client(self);
                                          }
                                      } else {
                                          self->server->disconnect_client(self);
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

std::string Connection::get_client_address() const {
    std::string remote_addr = socket.remote_endpoint().address().to_string();
    remote_addr.append(":") += std::to_string(socket.remote_endpoint().port());
    return remote_addr;
}
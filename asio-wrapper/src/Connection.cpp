#include "Connection.hpp"


Connection::Connection(boost::asio::ip::tcp::socket &&socket) : socket(std::move(socket)) {}

void Connection::start() {

    boost::asio::async_read_until(socket,
                                  streambuf,
                                  '\n',
                                  [self = shared_from_this()](boost::system::error_code error,
                                                              long bytes_transferred) {
                                      std::string command{
                                              buffers_begin(self->streambuf.data()),
                                              buffers_begin(self->streambuf.data()) + bytes_transferred
                                              - std::string("\n").size()};

                                      // Consume through the first delimiter so that subsequent async_read_until
                                      // will not reiterate over the same data.
                                      self->streambuf.consume(bytes_transferred);
                                      if (command.find("EXIT") == std::string::npos) {
                                          std::cout << "msg:" << command << '\n';
                                          self->start();
                                      } else {
                                          std::cout << "Disconnected" << '\n';
                                          self->socket.close();
                                      }
                                  });
}

Connection::~Connection() {
    std::cout<<"Connection destruktor"<<std::endl;
}

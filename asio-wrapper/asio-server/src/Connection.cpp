#include "Connection.hpp"


Connection::Connection(boost::asio::ip::tcp::socket &&socket) : socket(
        std::move(socket)) {
}

void Connection::start() {

    std::cout << "Waiting for message from client: " << socket.remote_endpoint().address() << ":"
              << socket.remote_endpoint().port() << "\n";
    boost::asio::async_read_until(socket,
                                  read_buf,
                                  '\n',
                                  [self = shared_from_this()](boost::system::error_code error,
                                                              long bytes_transferred) {
                                      std::unique_lock<std::shared_mutex> lock(self->message_shared_mutex);
                                      if (!error) {
                                          std::string command{
                                                  buffers_begin(self->read_buf.data()),
                                                  buffers_begin(self->read_buf.data()) + bytes_transferred
                                                  - std::string("\n").size()};

                                          self->read_buf.consume(bytes_transferred);
                                          self->messages.emplace_back(std::make_shared<Message>(self, command));
                                          self->start();
                                      } else {
                                          self->messages.emplace_back(
                                                  std::make_shared<Message>(self, ControlMessage::DISCONNECTED));
                                          self->disconnect();
                                      }
                                  });
}

Connection::~Connection() {
    std::cout << "Connection destruction" << "\n";
}

void Connection::sendMessage(std::unique_ptr<Message> &message) {
    write_buf.consume(write_buf.size());
    std::ostream request_stream(&write_buf);
    request_stream << message->get_message_text() << std::flush;
    boost::asio::write(socket, write_buf);
    write_buf.consume(write_buf.size());
    std::cout << "Message sent for client: " << socket.remote_endpoint().port() << "\n";

}

void Connection::disconnect() {
    if (socket.is_open()) {
        std::cout << "asio-client disconnected with ip: "
                  << socket.remote_endpoint().address() << " and port: "
                  << socket.remote_endpoint().port() << "\n";
        socket.close();
    }
}

std::vector<std::shared_ptr<Message>> Connection::read_messages() {
    std::unique_lock<std::shared_mutex> lock(message_shared_mutex);
    std::vector<std::shared_ptr<Message>> result(messages);
    messages.clear();
    return result;
}


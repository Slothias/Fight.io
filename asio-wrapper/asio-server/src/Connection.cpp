#include "Connection.hpp"


Connection::Connection(boost::asio::ip::tcp::socket &&socket, std::function<void(std::string)> &&t_message_handler,
                       std::function<void()> &&t_error_handler)
        : NetworkCallback(nullptr, nullptr, std::move(t_message_handler), std::move(t_error_handler)),
          socket(std::move(socket)) {
}

void Connection::async_read() {
    std::cout << "Waiting for message from client: " << socket.remote_endpoint().address() << ":"
              << socket.remote_endpoint().port() << "\n";
    boost::asio::async_read_until(socket,
                                  read_buf,
                                  '\n',
                                  [self = shared_from_this()](boost::system::error_code error,
                                                              long bytes_transferred) {
                                      if (!error) {
                                          std::string message{
                                                  buffers_begin(self->read_buf.data()),
                                                  buffers_begin(self->read_buf.data()) + bytes_transferred
                                                  - std::string("\n").size()};

                                          self->read_buf.consume(bytes_transferred);
                                          if (self->on_message_callback) {
                                              self->on_message_callback(message);
                                          }
                                          self->async_read();
                                      } else {
                                          if (self->on_error_callback) {
                                              self->on_error_callback();
                                          }
                                          self->disconnect();
                                      }
                                  });
}

void Connection::start() {
    if(on_join_callback){
        on_join_callback();
    }
    async_read();
}

Connection::~Connection() {
    std::cout << "Connection destruction" << "\n";
}

void Connection::sendMessage(std::unique_ptr<Message> &message) {
    boost::asio::write(socket, boost::asio::buffer(message->get_message_text()));
    std::cout << "Message sent for client: " << socket.remote_endpoint().port() << "\n";

}

void Connection::disconnect() {
    if (socket.is_open()) {
        std::cout << "asio-client disconnected with ip: "
                  << socket.remote_endpoint().address() << " and port: "
                  << socket.remote_endpoint().port() << "\n";
        if(on_leave_callback){
            on_leave_callback();
        }
        socket.close();
    }
}


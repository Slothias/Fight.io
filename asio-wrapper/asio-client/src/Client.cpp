//
// Created by sloy on 4/5/22.
//



#include "Client.hpp"

Client::Client(const std::string &host, std::uint16_t port) : io_context(*new boost::asio::io_context),
                                                              socket(io_context),
                                                              endpoint(boost::asio::ip::make_address(host), port) {

}


void Client::do_connect() {

    socket.async_connect(endpoint, [&](boost::system::error_code error) {
        if (!error) {
            std::cout << "Async connected!\n";
            async_read();
        } else {
            std::cerr<<error<<"\n";
            if (on_error_callback) {
                on_error_callback();
            }
        }
    });
    io_context.run();
}

void Client::async_read() {
    std::cout<<"Waiting data from server .... \n";
    boost::asio::async_read_until(socket,
                                  read_buf,
                                  '\n',
                                  [&](boost::system::error_code error,
                                      long bytes_transferred) {
                                      if (!error) {
                                          std::string message{
                                                  buffers_begin(read_buf.data()),
                                                  buffers_begin(read_buf.data()) + bytes_transferred
                                                  - std::string("\n").size()};

                                          read_buf.consume(bytes_transferred);
                                          if (on_message_callback) {
                                              on_message_callback(message);
                                          }
                                          async_read();
                                      } else {
                                          if (on_error_callback) {
                                              on_error_callback();
                                          }
                                      }
                                  });
}

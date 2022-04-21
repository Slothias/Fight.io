//
// Created by sloy on 4/5/22.
//



#include "AsioClient.hpp"

AsioClient::AsioClient(const std::string &host, std::uint16_t port) : endpoint(boost::asio::ip::make_address(host), port) {
}


void AsioClient::do_connect() {

    m_socket.async_connect(endpoint, [&](boost::system::error_code error) {
        if (!error) {
            std::cout << "Async connected!\n";
            async_read();
        } else {
            std::cerr << "Error happened! Code: " << error << "\n";
            if (on_error_callback) {
                on_error_callback();
            }
        }
    });
    m_io_context.run();
}

void AsioClient::async_read() {
    std::cout << "Waiting data from server .... \n";
    boost::asio::async_read_until(m_socket,
                                  m_read_buf,
                                  '\n',
                                  [&](boost::system::error_code error,long bytes_transferred)
                                  {
                                      if (!error) {
                                          std::string message{
                                                  buffers_begin(m_read_buf.data()),
                                                  buffers_begin(m_read_buf.data()) + bytes_transferred
                                                  - std::string("\n").size()};

                                          m_read_buf.consume(bytes_transferred);

                                          if (on_message_callback) {
                                              on_message_callback(message);
                                          }
                                          async_read();
                                      } else {
                                          if (on_error_callback) {
                                              on_error_callback();
                                          }
                                          std::cerr<<"Error happened! Code:"<<error<<"\n";
                                      }
                                  });
}

void AsioClient::send_message(std::string& message) {
    boost::asio::write(m_socket, boost::asio::buffer(message));

}

//
// Created by sloy on 4/4/22.
//

#include "Message.hpp"


Message::Message(std::shared_ptr<ServerAgent> conn, std::string t_message_body) : m_connection(std::move(conn)),
                                                                                  m_message_body(
                                                                                         std::move(t_message_body)),
                                                                                  m_timestamp(
                                                                                         boost::posix_time::microsec_clock::local_time()),
                                                                                  m_cast(UNICAST) {}

Message::Message(std::string t_message_body) : m_connection(nullptr),
                                               m_message_body(
                                                                  std::move(
                                                                          t_message_body)),
                                               m_timestamp(
                                                                  boost::posix_time::microsec_clock::local_time()),
                                               m_cast(BROADCAST) {}

std::string Message::get_message_text() const {
    return m_message_body;
}

boost::posix_time::ptime Message::get_timestamp() const {
    return m_timestamp;
}

std::shared_ptr<ServerAgent> Message::get_connection() const {
    return m_connection;
}

Cast Message::get_cast() const {
    return m_cast;
}
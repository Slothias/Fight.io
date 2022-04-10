//
// Created by sloy on 4/4/22.
//

#ifndef FIGHT_IO_MESSAGE_HPP
#define FIGHT_IO_MESSAGE_HPP

#include <boost/uuid/uuid.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>

enum Cast {
    UNICAST,BROADCAST
};

class Connection;
class Message:  public std::enable_shared_from_this<Message>{
private:
    std::string m_message_body;
    boost::posix_time::ptime m_timestamp;
    std::shared_ptr<Connection> m_connection;
    Cast m_cast;

public:
    Message(std::shared_ptr<Connection> conn, std::string t_message_body);
    Message(std::string t_message_body);

    std::string get_message_text() const;
    std::shared_ptr<Connection> get_connection() const;
    boost::posix_time::ptime get_timestamp() const;
    Cast get_cast() const;

    ~Message()=default;

};


#endif //FIGHT_IO_MESSAGE_HPP

#ifndef FIGHT_IO_MESSAGE_HPP
#define FIGHT_IO_MESSAGE_HPP

#include <boost/uuid/uuid.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
#include <boost/serialization/access.hpp>


enum Cast {
    UNICAST,BROADCAST
};

class ServerAgent;
class Message:  public std::enable_shared_from_this<Message>{
private:
    std::string m_message_body;
    boost::posix_time::ptime m_timestamp;
    std::shared_ptr<ServerAgent> m_connection;
    Cast m_cast;

public:
    Message(std::shared_ptr<ServerAgent> conn, std::string t_message_body);
    explicit Message(std::string t_message_body);

    std::string get_message_text() const;
    std::shared_ptr<ServerAgent> get_connection() const;
    boost::posix_time::ptime get_timestamp() const;
    Cast get_cast() const;

    ~Message()=default;

};


#endif //FIGHT_IO_MESSAGE_HPP

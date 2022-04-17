//
// Created by sloy on 2022.04.17..
//

#ifndef FIGHT_IO_NETWORKCALLBACK_HPP
#define FIGHT_IO_NETWORKCALLBACK_HPP


#include <functional>
#include <string>

class NetworkCallback {
protected:
    std::function<void()> on_join_callback;
    std::function<void()> on_leave_callback;
    std::function<void(std::string)> on_message_callback;
    std::function<void()> on_error_callback;
public:
    NetworkCallback() = default;
    NetworkCallback(std::function<void()>&& t_on_join_callback, std::function<void()>&& t_on_leave_callback,std::function<void(std::string)>&& t_on_message_callback,std::function<void()>&& t_on_error_callback);
    void set_on_join_callback(std::function<void()>&& t_on_join_callback);
    void set_on_leave_callback(std::function<void()>&& t_on_leave_callback);
    void set_on_message_callback(std::function<void(std::string)>&& t_on_message_callback);
    void set_on_error_callback(std::function<void()>&& t_on_error_callback);
};


#endif //FIGHT_IO_NETWORKCALLBACK_HPP

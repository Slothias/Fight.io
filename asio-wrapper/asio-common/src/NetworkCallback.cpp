//
// Created by sloy on 2022.04.17..
//

#include "NetworkCallback.hpp"

NetworkCallback::NetworkCallback(std::function<void()> &&t_on_join_callback,
                                 std::function<void()> &&t_on_leave_callback,
                                 std::function<void(std::string)> &&t_on_message_callback,
                                 std::function<void()> &&t_on_error_callback) :
        on_join_callback(std::move(t_on_join_callback)),
        on_leave_callback(std::move(t_on_leave_callback)),
        on_message_callback(std::move(t_on_message_callback)),
        on_error_callback(std::move(t_on_error_callback)) {

}

void NetworkCallback::set_on_join_callback(std::function<void()> &&t_on_join_callback) {
    on_join_callback = std::move(t_on_join_callback);
}

void NetworkCallback::set_on_leave_callback(std::function<void()> &&t_on_leave_callback) {
    on_join_callback = std::move(t_on_leave_callback);
}

void NetworkCallback::set_on_message_callback(std::function<void(std::string)> &&t_on_message_callback) {
    on_message_callback = t_on_message_callback;
}
void NetworkCallback::set_on_error_callback(std::function<void()> &&t_on_error_callback) {
    on_error_callback = t_on_error_callback;
}

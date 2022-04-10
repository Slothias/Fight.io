#include "Server.hpp"
#include <vector>

int main() {
    Server srv(15001);
    while (true) {
        std::vector<std::shared_ptr<Message>> messages = srv.read_all_messages();
        std::for_each(messages.begin(), messages.end(), [&srv](const std::shared_ptr<Message> &message) {
            if (message->get_message_text() != ControlMessage::DISCONNECTED) {
                std::cout << "Message arrived at: " << message->get_timestamp().date() << ", text: "
                          << message->get_message_text() << "\n";

            } else {
                srv.disconnect_client(message->get_connection());
            }
        });
    }
    return 0;
}
#include "Server.hpp"
#include <vector>

int main() {
    Server srv(15001);
    srv.start_listen();
    return 0;
}
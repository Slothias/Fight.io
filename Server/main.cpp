#include "ServerGui.hpp"

int main()
{
    {
    auto* s = new ServerGui();
    s->loop();
    }
    return 0;
}

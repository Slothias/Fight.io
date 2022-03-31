#include "ServerGui.hpp"

int main(void)
{
    {
    ServerGui* s = new ServerGui();
    s->loop();
    }
    return 0;
}

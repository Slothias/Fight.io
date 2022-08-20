#include "ServerUI.hpp"
#include<iostream>

int main(){
    std::unique_ptr<ServerUI> window = std::make_unique<ServerUI>(800,600,"Románkutya");
    window->showWindow();

    return 0;
}
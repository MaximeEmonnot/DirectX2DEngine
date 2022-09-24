#include <iostream>
#include <signal.h>
#include "Server.h"

void sigpipe_handle(int signal)
{
    std::cout << "A disconnection has occurred." << std::endl;
}

int main()
{
    sigaction(SIGPIPE, reinterpret_cast<struct sigaction*>(&sigpipe_handle), nullptr);
        
    Server server;
    while (true) server.Update();
    return 0;
}
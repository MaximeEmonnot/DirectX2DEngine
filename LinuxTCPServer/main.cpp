#include <iostream>
#include "TCPServer.h"

#include <csignal>

void handler(int sig)
{
    std::cout << "SigPipe received!" << std::endl;
}

int main()
{
    signal(SIGPIPE, handler);

    try {
        TCPServer server;
        while (true) server.Update();
    }
    catch(std::exception& e)
    {
        std::cerr << "An exception has occurred : " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "An unknown exception has occurred." << std::endl;
    }
    return 0;
}
#include <iostream>

#include "TCPServer.h"

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	try
	{
		TCPServer server;
		while (true) server.Update();
	}
	catch (std::exception& e)
	{
		std::cerr << "An exception has occurred : " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "An unknown exception has occurred." << std::endl;
	}
}

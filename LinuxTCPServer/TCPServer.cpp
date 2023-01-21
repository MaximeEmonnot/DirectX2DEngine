#include "TCPServer.h"

#include <iostream>

TCPServer::TCPServer()
{
	std::cout << "######### RUNNING SERVER ###########" << std::endl;
	std::cout << "Please, enter a port number : ";
	std::cin >> port;

	std::cout << "Please, enter the maximum client number per session : ";
	std::cin >> mMaxClients;

	server.InitServer(port);
}

TCPServer::~TCPServer()
{
	server.Close();
	FD_ZERO(&sockets); 
}

void TCPServer::Update()
{
	// NEW
	TCPSocket client = server.Accept();

	bool bClientIsAdded = false;

	while (!bClientIsAdded) {

		for (auto& entry : sessions)
		{
			if (entry.second != static_cast<int>(mMaxClients))
			{
				// Update Session number
				entry.second++;

				// Send Port + Place
				std::vector<uint8_t> data;
				data.push_back(static_cast<uint8_t>(entry.second));

				int udpPort = entry.first.GetPort();
				const uint8_t* portArray = reinterpret_cast<uint8_t*>(&udpPort);
				for (int i = 0; i < 4; i++)
					data.push_back(portArray[i]);
				client.SendData(data);

				// Client is added
				bClientIsAdded = true;
				break;
			}
		}

		// If client still not added, we create a new session
		if (!bClientIsAdded)
			sessions.emplace(std::pair<UDPServer, int>(UDPServer(++port), 0));
	}
}

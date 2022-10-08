#include "Server.h"

#include <iostream>

Server::Server()
{
	int port = 0;
	std::cout << "######### RUNNING SERVER ###########" << std::endl;
	std::cout << "Please, enter a port number : ";
	std::cin >> port;

	std::cout << "Please, enter the maximum client number : ";
	std::cin >> mMaxClients;

	server.InitServer(port);
}

Server::~Server()
{
	server.Close();
	FD_ZERO(&sockets); 
}

void Server::Update()
{
	FD_ZERO(&sockets);
	FD_SET(server, &sockets);
	for (TCPSocket& client : clients) FD_SET(client, &sockets);
	dataReceived.clear();

	select(128, &sockets, nullptr, nullptr, nullptr);

	if (FD_ISSET(server, &sockets))
	{
		TCPSocket client = server.Accept();
		if (clients.size() < mMaxClients) {
			clients.emplace_back(client);
			FD_SET(client, &sockets);
			std::cout << "New client connected !" << std::endl;

			// Send client's place
			std::vector<uint8_t> data;
			data.emplace_back(clients.size());
			client.SendData(data);
		}
		else
			std::cout << "Too many clients !" << std::endl;
	}
	else {
		// Receive Phase
		for (size_t i = 0; i < clients.size(); i++) {
			if (FD_ISSET(clients.at(i), &sockets))
			{
				const std::vector<uint8_t> data = clients.at(i).ReceiveData();
				if (!data.empty()) {
					dataReceived.emplace(std::pair<TCPSocket, std::vector<uint8_t>>(clients.at(i), data));
				}
				else {
					std::cout << "A client has disconnected" << std::endl;
					clients.at(i).Close();
					clients.erase(clients.begin() + static_cast<int>(i));
					break;
				}
			}
		}

		// Send Phase
		FD_ZERO(&sockets);
		for (TCPSocket& client : clients) FD_SET(client, &sockets);
		timeval timeout = {0, 5000};
		select(128, nullptr, &sockets, nullptr, &timeout);

		for (TCPSocket& client : clients)
			if (FD_ISSET(client, &sockets))
				for (auto& entry : dataReceived)
					if (client != entry.first) client.SendData(entry.second);
	}
}

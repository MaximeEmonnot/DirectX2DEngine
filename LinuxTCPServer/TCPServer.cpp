#include "TCPServer.h"

#include <iostream>

#include "ThreadPool.h"

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
	sessions.clear();

	server.Close();
	FD_ZERO(&sockets); 
}

void TCPServer::Update()
{
	FD_ZERO(&sockets);
	FD_SET(server, &sockets);
	for (TCPSocket& client : clients) FD_SET(client, &sockets);
	dataReceived.clear();

	select(128, &sockets, nullptr, nullptr, nullptr);

	if (FD_ISSET(server, &sockets))
	{
		TCPSocket client = server.Accept();
		clients.emplace_back(client);

		bool bClientIsAdded = false;
		bool bSessionIsCreated = false;

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
			if (!bClientIsAdded && !bSessionIsCreated) {
				THREAD.Enqueue([&]
				{
					UDPServer newSession(++port);
					sessions.emplace(newSession, 0);
					while (true) newSession.Update();
				});
				bSessionIsCreated = true;
			}
		}
	}
	else {
		std::cout << "Recieved data ! " << std::endl;

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
		timeval timeout = { 0, 5000 };
		select(128, nullptr, &sockets, nullptr, &timeout);

		for (TCPSocket& client : clients)
			if (FD_ISSET(client, &sockets))
				for (auto& entry : dataReceived)
					if (client != entry.first) client.SendData(entry.second);
	}
}

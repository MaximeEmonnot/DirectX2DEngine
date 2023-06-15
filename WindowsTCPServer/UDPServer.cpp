#include "UDPServer.h"

#include <iostream>
#include <set>

UDPServer::UDPServer(int port)
{
	std::cout << "##### RUNNING SESSION #####" << std::endl;

	server.InitServer(port);
}

UDPServer::~UDPServer()
{
	server.Close();
}

void UDPServer::Update()
{
	sockaddr_in client = {};
	char* data = server.ReceiveDataFrom(client);

	if (data && data[0] == 20) {
		clients.emplace(static_cast<int>(data[1]), client);
		for (auto& entry : clients)
			server.SendDataTo(data, entry.second);
	}
}

int UDPServer::GetPort() const
{
	return server.GetPort();
}

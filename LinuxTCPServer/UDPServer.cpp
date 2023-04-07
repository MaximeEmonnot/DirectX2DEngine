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
	std::vector<uint8_t> data = server.ReceiveDataFrom(client);

	if (!data.empty()) std::cout << data.size() << std::endl;

	if (!data.empty() && data.at(0) == 20)
		clientsData.insert(std::pair<sockaddr_in, std::vector<uint8_t>>(client, data));

	std::set<std::vector<uint8_t>> dataToSend;
	for (auto& entry : clientsData)
	{
		std::vector<uint8_t> clientData;
		clientData.push_back(static_cast<uint8_t>(11));
		for (int i = 1; i < 10; i++) clientData.push_back(entry.second.at(i));

		dataToSend.insert(clientData);
	}

	for (std::pair<sockaddr_in, std::vector<uint8_t>> entry : clientsData)
		for (auto& data : dataToSend)
			server.SendDataTo(data, entry.first);
}

int UDPServer::GetPort() const
{
	return server.GetPort();
}

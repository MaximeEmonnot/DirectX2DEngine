#pragma once
#include <map>
#include <vector>
#include "TCPSocket.h"
#include "UDPServer.h"

class TCPServer
{
public:
	TCPServer();
	~TCPServer();

	void Update();
private:
	TCPSocket server;

	int port = 0;

	std::map<UDPServer, int, UDPServer::ComparisonServer> sessions;

	std::vector<TCPSocket> clients;
	FD_SET sockets;
	unsigned long mMaxClients;

	std::map<TCPSocket, std::vector<uint8_t>> dataReceived;
};


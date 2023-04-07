#pragma once
#include <sys/select.h>
#include <map>
#include <thread>

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
	fd_set sockets;
	unsigned long mMaxClients;

	std::map<TCPSocket, std::vector<uint8_t>> dataReceived;
};


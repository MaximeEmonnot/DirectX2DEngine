#pragma once
#include <sys/select.h>
#include <map>

#include "TCPSocket.h"

class Server
{
public:
	Server();
	~Server();

	void Update();
private:
	TCPSocket server;
	std::vector<TCPSocket> clients;
	fd_set sockets;
	unsigned long mMaxClients;
	std::map<TCPSocket, std::vector<uint8_t>> dataReceived;
};


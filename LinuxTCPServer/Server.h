#pragma once
#include <sys/select.h>

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
};


module;
#include <WinSock2.h>

export module UDPServer;

import <map>;
import <string>;
import <iostream>;

import UDPSocket;
import Timer;

export class UDPServer
{
public:
	class ComparisonServer
	{
	public:
		bool operator () (const UDPServer& lhs, const UDPServer& rhs) const
		{
			return lhs.server.GetPort() < rhs.server.GetPort();
		}
	};

	class ComparisonClient
	{
	public:
		bool operator() (const sockaddr_in& lhs, const sockaddr_in& rhs) const
		{
			return lhs.sin_addr.s_addr < rhs.sin_addr.s_addr;
		}
	};

public:
	UDPServer() = default;
	UDPServer(int port)
	{
		std::cout << "##### RUNNING SESSION #####" << std::endl;

		server.InitServer(port);
	}
	~UDPServer()
	{
		server.Close();
	}

	void Update()
	{
		sockaddr_in client = {};
		char* data = server.ReceiveDataFrom(client);

		if (data && data[0] == 20) {
			clients.emplace(static_cast<uint8_t>(data[1]), client);
			for (auto& entry : clients)
				TIMER.MeasureExecution([&] {server.SendDataTo(data, entry.second); });
		}
	}

	int GetPort() const
	{
		return server.GetPort();
	}

private:
	UDPSocket server;

	std::map<uint8_t, sockaddr_in> clients;
};


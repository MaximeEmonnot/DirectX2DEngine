#pragma once
#include <map>

#include "UDPSocket.h"

class UDPServer
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
	UDPServer(int port);
	~UDPServer();


	void Update();

	int GetPort() const;

private:
	UDPSocket server;

	std::map<sockaddr_in, std::vector<uint8_t>, ComparisonClient> clientsData;
};


#pragma once
#include <WinSock2.h>
#include <cstdint>
#include <vector>

class UDPSocket
{
public:
	UDPSocket() = default;
	UDPSocket(int socket);
	UDPSocket& operator=(const UDPSocket& rhs);

	operator int() const;
	operator int& ();

	void InitServer(int port);
	void Close() const;

	int GetPort() const;

	void SendDataTo(char* data, sockaddr_in& dest) const;
	char* ReceiveDataFrom(sockaddr_in& source) const;

private:
	int mSocketDescriptor = -1;
	int port = 0;
};


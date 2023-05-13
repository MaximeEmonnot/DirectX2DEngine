#pragma once
#include <string>
#include <WinSock2.h>
#include <vector>

class TCPSocket
{
public :
	TCPSocket() = default;
	TCPSocket(unsigned long long socket);
	TCPSocket& operator=(const TCPSocket& rhs);
	
	operator unsigned long long() const;
	operator unsigned long long& ();

	void InitServer(int port);
	TCPSocket Accept() const;
	void Close() const;

	void SendData(std::vector<uint8_t> data) const;
	std::vector<uint8_t> ReceiveData() const;

private:
	SOCKET mSocketDescriptor = -1;
};


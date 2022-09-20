#pragma once
#include <cstdint>
#include <vector>


class TCPSocket
{
public:
	TCPSocket() = default;
	TCPSocket(int socket);
	TCPSocket& operator=(const TCPSocket& rhs);

	operator int() const;
	operator int& ();

	void InitServer(int port);
	TCPSocket Accept() const;
	void Close() const;

	void SendData(std::vector<uint8_t> data) const;
	std::vector<uint8_t> RecieveData() const;

private:
	int mSocketDescriptor = -1;
};


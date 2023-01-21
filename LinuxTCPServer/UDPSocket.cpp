#include "UDPSocket.h"

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

UDPSocket::UDPSocket(int socket)
	:
	mSocketDescriptor(socket)
{
}

UDPSocket& UDPSocket::operator=(const UDPSocket& rhs)
{
	mSocketDescriptor = rhs.mSocketDescriptor;
	return *this;
}

UDPSocket::operator int() const
{
	return mSocketDescriptor;
}

UDPSocket::operator int& ()
{
	return mSocketDescriptor;
}

void UDPSocket::InitServer(int port)
{
	this->port = port;

	mSocketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in server_address = {};
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(static_cast<uint16_t>(port));
	server_address.sin_addr.s_addr = INADDR_ANY;
	bind(mSocketDescriptor, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address));
}

void UDPSocket::Close() const
{
	close(mSocketDescriptor);
}

int UDPSocket::GetPort() const
{
	return port;
}

void UDPSocket::SendDataTo(std::vector<uint8_t> data, sockaddr_in& dest) const
{
	sendto(mSocketDescriptor, data.data(), data.size(), 0, reinterpret_cast<sockaddr*>(&dest), sizeof(dest));
}

std::vector<uint8_t> UDPSocket::ReceiveDataFrom(sockaddr_in& source) const
{
	std::vector<uint8_t> out;

	uint8_t* buffer = new uint8_t[1024];
	socklen_t addr_size = sizeof(source);
	recvfrom(mSocketDescriptor, buffer, 1024, 0, reinterpret_cast<sockaddr*>(&source), &addr_size);

	for (int i = 0; i < buffer[0]; i++) out.push_back(buffer[i]);

	return out;
}

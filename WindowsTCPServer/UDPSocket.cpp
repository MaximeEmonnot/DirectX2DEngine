#include "UDPSocket.h"

#include <iostream>
#include <string>

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

	if ((mSocketDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) std::cout << "Error socket creation!" << std::endl;
	sockaddr_in server_address = {};
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(static_cast<uint16_t>(port));
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(mSocketDescriptor, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) < 0) std::cout << "Error bind!" << std::endl;

	const int timeOut = 500;
	if (setsockopt(mSocketDescriptor, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&timeOut), sizeof(timeOut)) < 0) 
		std::cout << "Error setsockopt!" << std::endl;
	std::cout << "Listening on port : " << port << "\nSocket Descriptor : " << mSocketDescriptor << std::endl;
}

void UDPSocket::Close() const
{
	closesocket(mSocketDescriptor);
}

int UDPSocket::GetPort() const
{
	return port;
}

void UDPSocket::SendDataTo(char* data, sockaddr_in& dest) const
{
	if (sendto(mSocketDescriptor, data, 20, 0, reinterpret_cast<sockaddr*>(&dest), sizeof(dest)) == SOCKET_ERROR)
		std::cout << "An exception has been caught during UDP Data send : " << WSAGetLastError() << std::endl;

}

char* UDPSocket::ReceiveDataFrom(sockaddr_in& source) const
{
	char* buffer = new char[32];
	int addr_size = sizeof(source);
	if (recvfrom(mSocketDescriptor, buffer, 32, 0, reinterpret_cast<sockaddr*>(&source), &addr_size) == SOCKET_ERROR)
		std::cout << "An exception has been caught during UDP Data receive : " << WSAGetLastError() << std::endl;

	return buffer;
}

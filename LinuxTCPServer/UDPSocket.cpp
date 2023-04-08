#include "UDPSocket.h"

#include <bits/stdc++.h>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 500'000; // 500ms
	if (setsockopt(mSocketDescriptor, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) std::cout << "Error setsockopt!" << std::endl;

	std::cout << "Listening on port : " << port << "\nSocket Descriptor : " << mSocketDescriptor << std::endl;
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
	ssize_t size = sendto(mSocketDescriptor, data.data(), data.size(), MSG_CONFIRM, reinterpret_cast<sockaddr*>(&dest), sizeof(dest));
	std::cout << "Sent : " << size << " bytes" << std::endl;
}

std::vector<uint8_t> UDPSocket::ReceiveDataFrom(sockaddr_in& source) const
{
	std::vector<uint8_t> out;

	uint8_t* buffer = new uint8_t[1024];
	socklen_t addr_size = sizeof(source);

	if (recvfrom(mSocketDescriptor, buffer, 1024, MSG_WAITALL, reinterpret_cast<sockaddr*>(&source), &addr_size) < 0) {
		std::cout << "Erreur recv : " << errno << std::endl;
	}

	for (int i = 0; i < buffer[0]; i++) out.push_back(buffer[i]);

	return out;
}

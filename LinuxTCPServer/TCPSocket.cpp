#include "TCPSocket.h"

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

TCPSocket::TCPSocket(int socket)
	:
	mSocketDescriptor(socket)
{
}

TCPSocket& TCPSocket::operator=(const TCPSocket& rhs)
{
	mSocketDescriptor = rhs.mSocketDescriptor;
	return *this;
}

TCPSocket::operator int() const
{
	return mSocketDescriptor;
}

TCPSocket::operator int& ()
{
	return mSocketDescriptor;
}

void TCPSocket::InitServer(int port)
{
	mSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in server_address = {};
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(port);
	bind(mSocketDescriptor, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address));
	listen(mSocketDescriptor, SOMAXCONN);
}

TCPSocket TCPSocket::Accept() const
{
	return accept(mSocketDescriptor, nullptr, nullptr);
}

void TCPSocket::Close() const
{
	close(mSocketDescriptor);
}

void TCPSocket::SendData(std::vector<uint8_t> data) const
{
	const int size = static_cast<int> (data.size());
	if (send(mSocketDescriptor, &size, sizeof(int), 0) == 0)
		std::cout << "Error sending size" << std::endl;
	if (send(mSocketDescriptor, data.data(), size, 0) == 0)
		std::cout << "Error sending msg" << std::endl;
	std::cout << "Message sent. Size : " << data.size() << std::endl;
}

std::vector<uint8_t> TCPSocket::RecieveData() const
{
	std::vector<uint8_t> out;
	int size = 0;
	if (recv(mSocketDescriptor, &size, sizeof(int), 0) == -1) {
		std::cout << "Error recieving size" << std::endl;
	}
	else
	{
		char* buffer = new char[size + 1];
		if (recv(mSocketDescriptor, buffer, size, 0) == -1)
		{
			std::cout << "Error recieving msg" << std::endl;
			delete[] buffer;
		}
		else
		{
			for (int i = 0; i < size; i++) 
				out.emplace_back(static_cast<uint8_t>(buffer[i]));
			delete[] buffer;
		}
	}
	return out;
}

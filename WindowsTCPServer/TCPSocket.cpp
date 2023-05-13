#include "TCPSocket.h"

#include <iostream>
#include <string>
#include <WinSock2.h>

TCPSocket::TCPSocket(unsigned long long socket)
	:
	mSocketDescriptor(socket)
{}

TCPSocket& TCPSocket::operator=(const TCPSocket& rhs)
{
	mSocketDescriptor = rhs.mSocketDescriptor;
	return *this;
}

TCPSocket::operator unsigned long long() const
{
	return mSocketDescriptor;
}

TCPSocket::operator unsigned long long&()
{
	return mSocketDescriptor;
}

void TCPSocket::InitServer(int port)
{
	mSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in server_address = {};
	server_address.sin_family = AF_INET;
	server_address.sin_addr.S_un.S_addr = INADDR_ANY;
	server_address.sin_port = htons(static_cast<uint16_t>(port));
	if (bind(mSocketDescriptor, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) == SOCKET_ERROR)
		throw std::exception("Error during Server Binding!");
	if (listen(mSocketDescriptor, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception("Error during Socket Listening!");
}

TCPSocket TCPSocket::Accept() const
{
	return accept(mSocketDescriptor, nullptr, nullptr);
}

void TCPSocket::Close() const
{
	closesocket(mSocketDescriptor);
}

void TCPSocket::SendData(std::vector<uint8_t> data) const
{
	// We add the package size at the beginning of it
	int size = static_cast<int>(data.size());
	uint8_t* size_array = reinterpret_cast<uint8_t*>(&size);
	std::vector<uint8_t> size_vector(size_array, size_array + 4);
	data.insert(data.begin(), size_vector.begin(), size_vector.end());

	// Then we send it all at once
	if (send(mSocketDescriptor, reinterpret_cast<char*>(data.data()), data.size(), 0) == SOCKET_ERROR)
		std::cerr << "Error while sending message!\nMore infos : " + std::to_string(WSAGetLastError());
}

std::vector<uint8_t> TCPSocket::ReceiveData() const
{
	std::vector<uint8_t> out;
	int size = 0;
	// First we receive the package's size
	if (recv(mSocketDescriptor, reinterpret_cast<char*>(&size), sizeof(int), 0) == SOCKET_ERROR)
		std::cerr << std::string("Error while recieving size!\nMore infos : ") + std::to_string(WSAGetLastError()) << std::endl;
	else {
		// Then we receive the package
		char* buffer = new char[size + 1];
		if (recv(mSocketDescriptor, buffer, size, 0) == SOCKET_ERROR) {
			std::cerr << std::string("Error while receiving message!\nMore infos : ") + std::to_string(WSAGetLastError()) << std::endl;
			delete[] buffer;
		}
		else {
			for (int i = 0; i < size; i++)
				out.emplace_back(static_cast<uint8_t>(buffer[i]));
			delete[] buffer;
		}
	}
	return out;
}

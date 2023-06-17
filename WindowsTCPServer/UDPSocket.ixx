module;
#include <WinSock2.h>

export module UDPSocket;

import <iostream>;

export class UDPSocket
{
public:
	UDPSocket() = default;
	UDPSocket(int socket)
		:
		mSocketDescriptor(socket)
	{}
	UDPSocket& operator=(const UDPSocket& rhs)
	{
		mSocketDescriptor = rhs.mSocketDescriptor;
		return *this;
	}

	operator unsigned long long() const
	{
		return mSocketDescriptor;
	}
	operator unsigned long long& ()
	{
		return mSocketDescriptor;
	}

	void InitServer(int serverPort)
	{
		port = serverPort;

		if ((mSocketDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) std::cout << "Error socket creation!" << std::endl;
		sockaddr_in server_address = {};
		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(static_cast<uint16_t>(port));
		server_address.sin_addr.S_un.S_addr = ADDR_ANY;
		if (bind(mSocketDescriptor, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) == SOCKET_ERROR) std::cout << "Error bind!" << std::endl;

		const int timeOut = 500;
		if (setsockopt(mSocketDescriptor, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&timeOut), sizeof(timeOut)) == SOCKET_ERROR)
			std::cout << "Error setsockopt!" << std::endl;
		std::cout << "Listening on port : " << port << "\nSocket Descriptor : " << mSocketDescriptor << std::endl;
	}
	void Close() const
	{
		closesocket(mSocketDescriptor);
	}

	int GetPort() const
	{
		return port;
	}

	void SendDataTo(char* data, sockaddr_in& dest) const
	{
		if (sendto(mSocketDescriptor, data, 20, 0, reinterpret_cast<sockaddr*>(&dest), sizeof(dest)) == SOCKET_ERROR)
			std::cout << "An exception has been caught during UDP Data send : " << WSAGetLastError() << std::endl;
	}
	char* ReceiveDataFrom(sockaddr_in& source) const
	{
		char* buffer = new char[32];
		int addr_size = sizeof(source);
		if (recvfrom(mSocketDescriptor, buffer, 32, 0, reinterpret_cast<sockaddr*>(&source), &addr_size) == SOCKET_ERROR)
			std::cout << "An exception has been caught during UDP Data receive : " << WSAGetLastError() << std::endl;

		return buffer;
	}

private:
	SOCKET mSocketDescriptor;
	int port = 0;
};


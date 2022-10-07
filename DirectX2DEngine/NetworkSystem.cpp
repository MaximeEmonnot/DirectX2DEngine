#include "NetworkSystem.h"

#include "LoggerManager.h"
#include "Timer.h"

std::unique_ptr<NetworkSystem> NetworkSystem::pInstance = nullptr;

NetworkSystem::NetworkSystem()
	:
	sock(INVALID_SOCKET)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		throw NETWORK_EXCEPTION("An exception has been caught during WSA Startup.", WSAGetLastError());
}

NetworkSystem::~NetworkSystem()
{
	Disconnect();
}

NetworkSystem& NetworkSystem::GetInstance()
{
	if (!pInstance) pInstance = std::make_unique<NetworkSystem>();
	return *pInstance;
}

void NetworkSystem::ConnectTo(const std::string& ip_address, int port)
{
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		throw NETWORK_EXCEPTION("An exception has been caught during TCP Socket Creation.", WSAGetLastError());

	SOCKADDR_IN addr_in;
	addr_in.sin_addr.S_un.S_addr = inet_addr(ip_address.c_str());
	addr_in.sin_port = htons(port);
	addr_in.sin_family = AF_INET;

	if (connect(sock, reinterpret_cast<sockaddr*>(&addr_in), sizeof(addr_in)) == SOCKET_ERROR)
		throw NETWORK_EXCEPTION("An error has been caught during TCP Socket Connection.", WSAGetLastError());

	// Receive place
	std::vector<uint8_t> input = ReceiveData();
	place = input.at(0);
}

void NetworkSystem::Disconnect() const
{
	shutdown(sock, SD_BOTH);
	closesocket(sock);
}

void NetworkSystem::SendData(std::vector<uint8_t> data) const
{
	int size = static_cast<int>(data.size());
	if (send(sock, reinterpret_cast<char*>(&size), sizeof(int), 0) == SOCKET_ERROR)
		LOG(std::string("Error while sending size!\nMore infos : ") + std::to_string(WSAGetLastError()), LOG_ERROR)
	if (send(sock, reinterpret_cast<char*>(data.data()), size, 0) == SOCKET_ERROR) 
		LOG(std::string("Error while sending message!\nMore infos : ") + std::to_string(WSAGetLastError()), LOG_ERROR)
}

std::vector<uint8_t> NetworkSystem::ReceiveData() const
{
	std::vector<uint8_t> out;
	int size = 0;
	if (recv(sock, reinterpret_cast<char*>(&size), sizeof(int), 0) == SOCKET_ERROR)
		LOG(std::string("Error while recieving size!\nMore infos : ") + std::to_string(WSAGetLastError()), LOG_ERROR)
	else {
		char* buffer = new char[size + 1];
		if (recv(sock, buffer, size, 0) == SOCKET_ERROR) {
			LOG(std::string("Error while receiving message!\nMore infos : ") + std::to_string(WSAGetLastError()), LOG_ERROR)
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

int NetworkSystem::GetPlace() const
{
	return place;
}

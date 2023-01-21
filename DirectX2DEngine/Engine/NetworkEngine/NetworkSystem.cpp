#include "NetworkEngine/NetworkSystem.h"

#include <deque>

#include "CoreEngine/Timer.h"
#include "DebugEngine/LoggerManager.h"

std::unique_ptr<NetworkSystem> NetworkSystem::pInstance = nullptr;

NetworkSystem::NetworkSystem()
	:
	socketTCP(INVALID_SOCKET)
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
	// TCP Socket Creation
	if ((socketTCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		throw NETWORK_EXCEPTION("An exception has been caught during TCP Socket Creation.", WSAGetLastError());

	// UDP Socket Creation
	if ((socketUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
		throw NETWORK_EXCEPTION("An exception has been caught during UDP Socket Creation.", WSAGetLastError());

	// Connection to the distant server
	SOCKADDR_IN addr_in;
	addr_in.sin_addr.S_un.S_addr = inet_addr(ip_address.c_str());
	addr_in.sin_port = htons(port);
	addr_in.sin_family = AF_INET;

	if (connect(socketTCP, reinterpret_cast<sockaddr*>(&addr_in), sizeof(addr_in)) == SOCKET_ERROR)
		throw NETWORK_EXCEPTION("An error has been caught during TCP Socket Connection.", WSAGetLastError());

	// We receive our place in the server, plus the port of the UDP session
	std::vector<uint8_t> input = ReceiveDataTCP();
	place = input.at(0);
	bIsOnline = true;
	const int portUDP = input.at(1);
	if (ResolveAddress(ip_address, AF_INET, std::to_string(port), &addrUDP) != 0)
		throw NETWORK_EXCEPTION("An exception has been caught during UDP Server Address Resolution", -1);
}

void NetworkSystem::Disconnect()
{
	bIsOnline = false;
	shutdown(socketTCP, SD_BOTH);
	closesocket(socketTCP);
	shutdown(socketUDP, SD_BOTH);
	closesocket(socketUDP);
}

void NetworkSystem::SendDataUDP(std::vector<uint8_t> data)
{
	sendto(socketUDP, reinterpret_cast<const char*>(data.data()), static_cast<int>(data.size()), 0, reinterpret_cast<sockaddr*>(&addrUDP), sizeof(addrUDP));
}

std::vector<uint8_t> NetworkSystem::ReceiveDataUDP()
{
	std::vector<uint8_t> out;

	uint8_t* buffer = new uint8_t[1024];
	int addr_size = sizeof(addrUDP);
	recvfrom(socketUDP, reinterpret_cast<char*>(buffer), 1024, 0, reinterpret_cast<sockaddr*>(&addrUDP), &addr_size);

	for (int i = 0; i < buffer[0]; i++) out.push_back(buffer[i]);

	return out;
}

void NetworkSystem::SendDataTCP(std::vector<uint8_t> data) const
{
	// We add the package size at the beginning of it
	int size = static_cast<int>(data.size());
	uint8_t* size_array = reinterpret_cast<uint8_t*>(&size);
	std::vector<uint8_t> size_vector(size_array, size_array + 4);
	data.insert(data.begin(), size_vector.begin(), size_vector.end());

	// Then we send it all at once
	if (send(socketTCP, reinterpret_cast<char*>(data.data()), data.size(), 0) == SOCKET_ERROR) 
		LOG(std::string("Error while sending message!\nMore infos : ") + std::to_string(WSAGetLastError()), LOG_ERROR)
}

std::vector<uint8_t> NetworkSystem::ReceiveDataTCP() const
{
	std::vector<uint8_t> out;
	int size = 0;
	// First we receive the package's size
	if (recv(socketTCP, reinterpret_cast<char*>(&size), sizeof(int), 0) == SOCKET_ERROR)
		LOG(std::string("Error while recieving size!\nMore infos : ") + std::to_string(WSAGetLastError()), LOG_ERROR)
	else {
		// Then we receive the package
		char* buffer = new char[size + 1];
		if (recv(socketTCP, buffer, size, 0) == SOCKET_ERROR) {
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

bool NetworkSystem::IsOnline() const
{
	return bIsOnline;
}

int NetworkSystem::ResolveAddress(const std::string& hostname, int family, const std::string& service,
	sockaddr_storage* pAddr)
{
	int result;
	addrinfo* result_list = nullptr;
	addrinfo hints = { };
	hints.ai_family = family;
	hints.ai_socktype = SOCK_DGRAM;
	result = getaddrinfo(hostname.c_str(), service.c_str(), &hints, &result_list);
	if (result == 0)
	{
		memcpy(pAddr, result_list->ai_addr, result_list->ai_addrlen);
		freeaddrinfo(result_list);
	}

	return result;
}

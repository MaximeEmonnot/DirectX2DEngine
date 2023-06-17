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
	addr_in.sin_port = htons(static_cast<u_short>(port));
	addr_in.sin_family = AF_INET;

	if (connect(socketTCP, reinterpret_cast<sockaddr*>(&addr_in), sizeof(addr_in)) == SOCKET_ERROR)
		throw NETWORK_EXCEPTION("An error has been caught during TCP Socket Connection.", WSAGetLastError());

	// We receive our place in the server, plus the port of the UDP session
	std::vector<uint8_t> input = ReceiveDataTCP();
	place = input.at(0);
	bIsOnline = true;

	uint8_t portUDP_array[4] = { input.at(1), input.at(2), input.at(3), input.at(4) };

	const int portUDP = *reinterpret_cast<int*>(portUDP_array);
	addrUDP.sin_family = AF_INET;
	addrUDP.sin_port = htons(static_cast<u_short>(portUDP));
	addrUDP.sin_addr.S_un.S_addr = inet_addr(ip_address.c_str());

	//if (place == 2) {
		const int timeOut = 500;
		if (setsockopt(socketUDP, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&timeOut), sizeof(timeOut)) < 0)
			throw NETWORK_EXCEPTION("An exception has been caught during UDP Timeout definition.", WSAGetLastError());
	//}
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
	if (sendto(socketUDP, reinterpret_cast<const char*>(data.data()), static_cast<int>(data.size()), 0, reinterpret_cast<sockaddr*>(&addrUDP), sizeof(addrUDP)) == SOCKET_ERROR)
		LOG(std::string("An exception has been caught during UDP Data send.") + std::to_string(WSAGetLastError()), LOG_ERROR)
}

std::vector<uint8_t> NetworkSystem::ReceiveDataUDP()
{
	std::vector<uint8_t> out;
	char* buffer = new char[1024];
	int addr_size = sizeof(addrUDP);
	if (recvfrom(socketUDP, buffer, 1024, 0, reinterpret_cast<sockaddr*>(&addrUDP), &addr_size) == SOCKET_ERROR)
		LOG(std::string("An exception has been caught during UDP Data receive.") + std::to_string(WSAGetLastError()), LOG_ERROR)
	for (int i = 0; i < buffer[0]; i++) out.push_back(static_cast<uint8_t>(buffer[i]));

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
	if (send(socketTCP, reinterpret_cast<char*>(data.data()), static_cast<int>(data.size()), 0) == SOCKET_ERROR) 
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

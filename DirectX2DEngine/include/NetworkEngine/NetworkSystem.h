#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <memory>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

#include "DebugEngine/EngineException.h"

#define NETWORK NetworkSystem::GetInstance()
#define NETWORK_EXCEPTION(note, error) NetworkSystem::NetworkException("DirectX 2D Engine - Network Engine Exception", note, __FILE__, __LINE__, error)

class NetworkSystem
{
public:
	class NetworkException : public EngineException
	{
	public:
		NetworkException(const std::string& type, const std::string& note, const std::string& file, unsigned int line, int error) noexcept
			:
			EngineException(type, note, file, line)
		{
			infos += "\nError code : " + std::to_string(error);
		}
	};
public:
	NetworkSystem();
	NetworkSystem(const NetworkSystem&) = delete;
	NetworkSystem& operator=(const NetworkSystem&) = delete;
	~NetworkSystem();

	static NetworkSystem&	GetInstance();

	// Connects the Client to a distance Server.
	void					ConnectTo(const std::string& ipAddress, int port);
	// Disconnects the client's socket.
	void					Disconnect();


	void					SendDataUDP(std::vector<uint8_t> data);
	std::vector<uint8_t>	ReceiveDataUDP();

	void					SendDataTCP(std::vector<uint8_t> data) const;
	std::vector<uint8_t>	ReceiveDataTCP() const;

	int						GetPlace() const;
	bool					IsOnline() const;

private:
	int ResolveAddress(const std::string& hostname, int family, const std::string& service, sockaddr_storage* pAddr);

private:
	static std::unique_ptr<NetworkSystem>	pInstance;

	SOCKET									socketTCP;
	SOCKET									socketUDP;
	bool									bIsOnline = false;
	int										place = 0;
	sockaddr_storage						addrUDP = {};
};


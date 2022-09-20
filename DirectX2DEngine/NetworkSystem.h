#pragma once

#include <WinSock2.h>
#include <memory>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

#include "EngineException.h"

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

	static NetworkSystem& GetInstance();

	void ConnectTo(const std::string& ip_address, int port);
	void Disconnect() const;

	void SendData(std::vector<uint8_t> data) const;
	std::vector<uint8_t> ReceiveData() const;

private:
	static std::unique_ptr<NetworkSystem> pInstance;

	SOCKET sock;
};


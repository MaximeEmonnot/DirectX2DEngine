#pragma once
#include "Logger.h"

#define LOG(message, level) LoggerManager::GetInstance().Log(message, level);
#define LOG_CONSOLE Logger::Level::CONSOLE
#define LOG_DEBUG Logger::Level::DEBUG
#define LOG_ERROR Logger::Level::ERR

class LoggerManager
{
public:
	LoggerManager();
	LoggerManager(const LoggerManager&) = delete;
	LoggerManager& operator= (const LoggerManager&) = delete;

	static LoggerManager& GetInstance();

	// Logs a message on the specified Logging Level.
	void Log(const std::string& message, Logger::Level level) const;

private:
	static std::unique_ptr<LoggerManager> pInstance;
	std::shared_ptr<Logger> pLog;
};


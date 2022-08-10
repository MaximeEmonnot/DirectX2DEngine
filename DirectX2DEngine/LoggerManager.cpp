#include "LoggerManager.h"

#include "ConsoleLogger.h"
#include "DebugLogger.h"
#include "ErrorLogger.h"

// STATIC VARIABLES INITIALIZATION
std::unique_ptr<LoggerManager> LoggerManager::pInstance = nullptr;

LoggerManager::LoggerManager()
	:
	log(std::make_shared<ConsoleLogger>())
{
	log->SetNext(std::make_shared<DebugLogger>())->SetNext(std::make_shared<ErrorLogger>());
}

LoggerManager& LoggerManager::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<LoggerManager>();
	return *pInstance;
}

void LoggerManager::Log(const std::string& message, Logger::Level level) const
{
	log->Write(message, level);
}

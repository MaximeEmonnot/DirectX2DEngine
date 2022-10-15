#include "DebugEngine/LoggerManager.h"

#include "DebugEngine/ConsoleLogger.h"
#include "DebugEngine/DebugLogger.h"
#include "DebugEngine/ErrorLogger.h"

// STATIC VARIABLES INITIALIZATION
std::unique_ptr<LoggerManager> LoggerManager::pInstance = nullptr;

//******//

LoggerManager::LoggerManager()
	:
	pLog(std::make_shared<ConsoleLogger>())
{
	pLog->SetNext(std::make_shared<DebugLogger>())->SetNext(std::make_shared<ErrorLogger>());
}

LoggerManager& LoggerManager::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<LoggerManager>();
	return *pInstance;
}

void LoggerManager::Log(const std::string& message, Logger::Level level) const
{
	pLog->Write(message, level);
}

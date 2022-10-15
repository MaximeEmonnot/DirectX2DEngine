#include "DebugEngine/Logger.h"

#include <filesystem>

// STATIC VARIABLES INITIALIZATION
std::string Logger::outDirectory;

Logger::Logger()
{
	const time_t now = time(0);
	const tm* ltm = localtime(&now);
	outDirectory = std::string("Logs/") + std::to_string(ltm->tm_mday) + std::string("-") + std::to_string(ltm->tm_mon + 1) + std::string("-") + std::to_string(ltm->tm_year + 1900);
	std::filesystem::create_directories(outDirectory);
}

std::shared_ptr<Logger> Logger::SetNext(std::shared_ptr<Logger> next)
{
	pNext = next;
	return pNext;
}

void Logger::Write(const std::string& message, Level priority)
{
	if (priority == level) LogMessage(message);
	if (pNext) pNext->Write(message, priority);
}

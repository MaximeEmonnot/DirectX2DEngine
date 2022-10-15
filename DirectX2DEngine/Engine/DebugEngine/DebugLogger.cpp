#include "DebugEngine/DebugLogger.h"

#include <Windows.h>
#include <chrono>

DebugLogger::DebugLogger()
{
	level = Level::DEBUG;
	out.open(outDirectory + "/DEBUG.txt", std::ios::out | std::ios::app);
}

DebugLogger::~DebugLogger()
{
	out.close();
}

void DebugLogger::LogMessage(const std::string& message)
{
	const __time64_t timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	const std::string out_message = std::string(std::ctime(&timestamp)) + std::string("[DEBUG] : ") + message + "\n";
	out << out_message;
	OutputDebugStringA(out_message.c_str());
}

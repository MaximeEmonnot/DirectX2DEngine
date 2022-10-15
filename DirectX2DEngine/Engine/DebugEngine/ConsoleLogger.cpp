#include "DebugEngine/ConsoleLogger.h"
#include <chrono>
#include <Windows.h>

ConsoleLogger::ConsoleLogger()
{
	level = Level::CONSOLE;
	out.open(outDirectory + "/CONSOLE.txt", std::ios::out | std::ios::app);
}

ConsoleLogger::~ConsoleLogger()
{
	out.close();
}

void ConsoleLogger::LogMessage(const std::string& message)
{
	const __time64_t timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	const std::string out_message = std::string(std::ctime(&timestamp)) + std::string("[CONSOLE] : ") + message + "\n";
	out << out_message;
	OutputDebugStringA(out_message.c_str());
}

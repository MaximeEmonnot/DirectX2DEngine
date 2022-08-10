#include "ErrorLogger.h"
#include <chrono>
#include <Windows.h>

ErrorLogger::ErrorLogger()
{
	level = Logger::Level::ERR;
	out.open(outDirectory + "/ERROR.txt", std::ios::out | std::ios::app);
}

ErrorLogger::~ErrorLogger()
{
	out.close();
}

void ErrorLogger::LogMessage(const std::string& message)
{
	const __time64_t timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	const std::string out_message = std::string(std::ctime(&timestamp)) + std::string("[ERROR] : ") + message + "\n";
	out << out_message;
	OutputDebugStringA(out_message.c_str());
}

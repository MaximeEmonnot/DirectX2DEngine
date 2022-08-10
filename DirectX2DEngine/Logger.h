#pragma once

#include <string>
#include <memory>

class Logger
{
public:
	enum class Level
	{
		CONSOLE,
		DEBUG,
		ERR,
		NONE
	};
public:
	Logger();
	virtual ~Logger() = default;

	std::shared_ptr<Logger> SetNext(std::shared_ptr<Logger> next);
	void Write(const std::string& message, Level priority);

protected:
	virtual void LogMessage(const std::string& message) = 0;

protected:
	static std::string outDirectory;
	Level level = Level::NONE;

	std::shared_ptr<Logger> pNext;
};


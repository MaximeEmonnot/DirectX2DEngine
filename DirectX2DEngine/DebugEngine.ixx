module;
#include <Windows.h>

export module DebugEngine;

import <filesystem>;
import <exception>;
import <sstream>;
import <fstream>;

export
class EngineException : public std::exception
{
public:
	EngineException(const std::string& type, const std::string& note, const std::string& file, unsigned int line) noexcept
		:
		type(type),
		note(note),
		file(file),
		line(line)
	{
		infos = GetNote() + std::string("\nDetails:\n") + GetLocation();
	}

	EngineException(const EngineException& copy) noexcept
		:
		EngineException(copy.type, copy.note, copy.file, copy.line)
	{}

	EngineException& operator=(const EngineException& engineException)
	{
		*this = EngineException(engineException);
		return *this;
	}
	EngineException(EngineException&& move) noexcept
		:
		infos(std::move(move.infos)),
		type(std::move(move.type)),
		note(std::move(move.note)),
		file(std::move(move.file)),
		line(move.line)
	{}
	EngineException& operator=(EngineException&& engineException) noexcept
	{
		*this = EngineException(engineException);
		return *this;
	}

	~EngineException() override = default;

	inline std::string GetType() const noexcept
	{
		return type;
	}

	inline std::string GetFile() const noexcept
	{
		return file;
	}

	inline std::string GetNote() const noexcept
	{
		return note;
	}

	inline unsigned int GetLine() const noexcept
	{
		return line;
	}

	inline std::string GetLocation() const noexcept
	{
		return std::string("[File]: \"") + file + std::string("\"\n[Line]: \"") + std::to_string(line);
	}

	const char* what() const override
	{
		return infos.c_str();
	}

protected:
	std::string		infos;
private:
	std::string		type;
	std::string		note;
	std::string		file;
	unsigned int	line;
};

export EngineException ENGINE_EXCEPTION(const std::string& type, const std::string& note)
{
	return EngineException(type, note, __FILE__, __LINE__);
}
export void SHOW_MESSAGE(const std::string& title, const std::string& message)
{
	MessageBoxExA(nullptr, message.c_str(), title.c_str(), MB_ICONERROR | MB_OK, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
}


export
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
	Logger()
	{
		const time_t now = time(0);
		tm ltm;
		localtime_s(&ltm, &now);
		outDirectory = std::string("Logs/") + std::to_string(ltm.tm_mday) + std::string("-") + std::to_string(ltm.tm_mon + 1) + std::string("-") + std::to_string(ltm.tm_year + 1900);
		std::filesystem::create_directories(outDirectory);
	}
	virtual ~Logger() = default;

	// Defines the next Logging Level
	std::shared_ptr<Logger>		SetNext(std::shared_ptr<Logger> pNextLog)
	{
		pNext = pNextLog;
		return pNext;
	}

	// Writes on the current Logging Level, or pass it to the next level.
	void						Write(const std::string& message, Level priority)
	{
		if (priority == level) LogMessage(message);
		if (pNext) pNext->Write(message, priority);
	}

protected:
	virtual void				LogMessage(const std::string& message) = 0;

protected:
	static std::string			outDirectory;
	Level						level = Level::NONE;

	std::shared_ptr<Logger>		pNext;
};
// STATIC VARIABLES INITIALIZATION
std::string Logger::outDirectory;


export
class ConsoleLogger :
	public Logger
{
public:
	ConsoleLogger()
	{
		level = Level::CONSOLE;
		out.open(outDirectory + "/CONSOLE.txt", std::ios::out | std::ios::app);
	}
	virtual ~ConsoleLogger() override
	{
		out.close();
	}

protected:
	void            LogMessage(const std::string& message) override
	{
		const __time64_t timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		const std::string out_message = std::string(std::ctime(&timestamp)) + std::string("[CONSOLE] : ") + message + "\n";
		out << out_message;
		OutputDebugStringA(out_message.c_str());
	}

private:
	std::ofstream   out;
};


export
class DebugLogger :
	public Logger
{
public:
	DebugLogger()
	{
		level = Level::DEBUG;
		out.open(outDirectory + "/DEBUG.txt", std::ios::out | std::ios::app);
	}
	virtual ~DebugLogger() override
	{
		out.close();
	}

protected:
	void            LogMessage(const std::string& message) override
	{
		const __time64_t timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		const std::string out_message = std::string(std::ctime(&timestamp)) + std::string("[DEBUG] : ") + message + "\n";
		out << out_message;
		OutputDebugStringA(out_message.c_str());
	}

private:
	std::ofstream   out;
};


export
class ErrorLogger :
	public Logger
{
public:
	ErrorLogger()
	{
		level = Logger::Level::ERR;
		out.open(outDirectory + "/ERROR.txt", std::ios::out | std::ios::app);
	}
	virtual ~ErrorLogger() override
	{
		out.close();
	}

protected:
	void            LogMessage(const std::string& message) override
	{
		const __time64_t timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		const std::string out_message = std::string(std::ctime(&timestamp)) + std::string("[ERROR] : ") + message + "\n";
		out << out_message;
		OutputDebugStringA(out_message.c_str());
	}

private:
	std::ofstream   out;
};


export
class LoggerManager
{
public:
	LoggerManager()
		:
		pLog(std::make_shared<ConsoleLogger>())
	{
		pLog->SetNext(std::make_shared<DebugLogger>())->SetNext(std::make_shared<ErrorLogger>());
	}
	LoggerManager(const LoggerManager&) = delete;
	LoggerManager& operator= (const LoggerManager&) = delete;

	static LoggerManager& GetInstance()
	{
		if (!pInstance)
			pInstance = std::make_unique<LoggerManager>();
		return *pInstance;
	}

	// Logs a message on the specified Logging Level.
	void					Log(const std::string& message, Logger::Level level) const
	{
		pLog->Write(message, level);
	}

private:
	static std::unique_ptr<LoggerManager>	pInstance;
	std::shared_ptr<Logger>					pLog;
};

// STATIC VARIABLES INITIALIZATION
std::unique_ptr<LoggerManager> LoggerManager::pInstance = nullptr;

export void LOG(const std::string& message, Logger::Level level)
{
	LoggerManager::GetInstance().Log(message, level);
}
export constexpr Logger::Level LOG_CONSOLE = Logger::Level::CONSOLE;
export constexpr Logger::Level LOG_DEBUG = Logger::Level::DEBUG;
export constexpr Logger::Level LOG_ERROR = Logger::Level::ERR;
#pragma once
#include <Windows.h>
#include <exception>
#include <sstream>

#define ENGINE_EXCEPTION(type, note) EngineException(type, note, __FILE__, __LINE__)
#define SHOW_MESSAGE(title, message) MessageBoxExA(nullptr, message, title, MB_ICONERROR | MB_OK, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));

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
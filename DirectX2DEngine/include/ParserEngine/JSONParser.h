#pragma once

#include "rapidjson/document.h"

#include "DebugEngine/EngineException.h"

#define JSON_EXCEPTION(note) JSONException("DirectX 2D Engine - JSON Engine Exception", note, __FILE__, __LINE__)

namespace JSONParser
{
	class JSONException : public EngineException
	{
	public:
		JSONException(const std::string& type, const std::string& note, const std::string& file, unsigned int line)
			:
			EngineException(type, note, file, line)
		{
		}
	};

	class Reader
	{
	public:
		Reader() = default;
		Reader(const Reader& copy);
		Reader& operator= (const Reader& copy);

		// Initializes the JSON Document.
		void				ReadFile(const std::string& filePath);

		// Reads any value using its name.
		rapidjson::Value&	GetValueOf(const std::string& valueName);

	private:
		rapidjson::Document	document;
	};
}


#pragma once
#include <map>
#include <memory>

#include "JSONParser.h"

#define JSON(path) JSONManager::GetInstance().GetFile(path)

class JSONManager
{
public:
	JSONManager() = default;
	JSONManager(const JSONManager&) = delete;
	JSONManager operator= (const JSONManager&) = delete;

	static JSONManager& GetInstance();

	JSONParser::Reader GetFile(const std::string& path);

private:
	static std::unique_ptr<JSONManager> pInstance;

	std::map<std::string, JSONParser::Reader> jsonFiles;
};

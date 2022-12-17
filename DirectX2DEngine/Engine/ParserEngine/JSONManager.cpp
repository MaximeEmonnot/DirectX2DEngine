#include "ParserEngine/JSONManager.h"

std::unique_ptr<JSONManager> JSONManager::pInstance = nullptr;

JSONManager& JSONManager::GetInstance()
{
	if (!pInstance) pInstance = std::make_unique<JSONManager>();
	return *pInstance;
}

JSONParser::Reader JSONManager::GetFile(const std::string& path)
{
	if (!jsonFiles.contains(path))
	{
		JSONParser::Reader fileParser;
		fileParser.ReadFile(path);
		jsonFiles.emplace(path, fileParser);
	}

	return jsonFiles.at(path);
}

#pragma once
#include <memory>
#include <vector>

#include "EngineException.h"
#include "Level.h"

#define ENGINE Engine::GetInstance()

class Engine
{
public:
	Engine() = default;
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	~Engine() = default;

	static Engine& GetInstance();

	void Update() const;
	void Render() const;

	void Clear();

	template<class T>
	void AddLevel()
	{
		levels.emplace_back(std::make_shared<T>());
		if (!pCurrentLevel) pCurrentLevel = levels.front();
	}

	template<class T>
	void SetLevel()
	{
		for (std::shared_ptr<Level> level : levels)
		{
			if (std::dynamic_pointer_cast<T>(level))
			{
				pCurrentLevel = level;
				return;
			}
		}
		throw ENGINE_EXCEPTION("DirectX 2D Engine - Standard exception", "This level doesn't exist! Please check your code.");
	}

	std::shared_ptr<Level> GetCurrentLevel() const;

private:
	static std::unique_ptr<Engine> pInstance;

	std::vector<std::shared_ptr<Level>> levels;
	std::shared_ptr<Level> pCurrentLevel;
};

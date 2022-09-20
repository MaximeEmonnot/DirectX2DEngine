#pragma once
#include <memory>
#include <vector>

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
	}

	void SetLevel(int newIndex);
	Level& GetCurrentLevel() const;

private:
	static std::unique_ptr<Engine> pInstance;

	std::vector<std::shared_ptr<Level>> levels;
	int levelIndex = 0;
};

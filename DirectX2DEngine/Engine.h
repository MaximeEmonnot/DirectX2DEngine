#pragma once
#include <map>
#include <memory>
#include <vector>

#include "BaseModel.h"
#include "Level.h"

#define ENGINE Engine::GetInstance()
#define LEVEL Engine::GetInstance().GetCurrentLevel()
#define WORLD Engine::GetInstance().GetCurrentLevel().GetWorld()

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

	template<class T>
	void AddLevel()
	{
		levels.emplace_back(std::make_shared<T>());
	}

	template<class T>
	std::shared_ptr<T> CreateModel(int priority)
	{
		std::shared_ptr<BaseModel> new_model = std::make_shared<T>();
		new_model->Initialize();
		models.emplace(std::pair(priority, new_model));
		return std::dynamic_pointer_cast<T>(new_model);
	}

	template<class T, class... Args>
	std::shared_ptr<T> CreateModel(int priority, Args... args)
	{
		std::shared_ptr<BaseModel> new_model = std::make_shared<T>(args...);
		new_model->Initialize();
		models.emplace(std::pair(priority, new_model));
		return std::dynamic_pointer_cast<T>(new_model);
	}

	void SetLevel(int newIndex);
	Level& GetCurrentLevel() const;

private:
	static std::unique_ptr<Engine> pInstance;

	std::multimap<int, std::shared_ptr<BaseModel>> models;
	std::vector<std::shared_ptr<Level>> levels;
	int levelIndex = 0;
};

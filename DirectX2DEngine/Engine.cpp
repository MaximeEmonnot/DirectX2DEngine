#include "Engine.h"

#include "Timer.h"

// STATIC VARIABLES INITIALIZATION

std::unique_ptr<Engine> Engine::pInstance = nullptr;

Engine& Engine::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<Engine>();
	return *pInstance;
}

void Engine::Update() const
{
	TICKCLOCK

	levels.at(levelIndex)->Update();
}

void Engine::Render() const
{
	for (const std::pair<const int, std::shared_ptr<BaseModel>>& entry : models)
		entry.second->Render();
}

void Engine::SetLevel(int newIndex)
{
	levelIndex = newIndex;
}

Level& Engine::GetCurrentLevel() const
{
	return *levels.at(levelIndex);
}

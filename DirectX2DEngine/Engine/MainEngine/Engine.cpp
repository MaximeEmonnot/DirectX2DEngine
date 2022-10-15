#include "MainEngine/Engine.h"

#include "CoreEngine/Timer.h"

// STATIC VARIABLES INITIALIZATION

std::unique_ptr<Engine> Engine::pInstance = nullptr;

//******//

Engine& Engine::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<Engine>();
	return *pInstance;
}

void Engine::Update() const
{
	TICKCLOCK

	if (pCurrentLevel) pCurrentLevel->Update();
}

void Engine::Render() const
{
	if (pCurrentLevel) pCurrentLevel->Render();
}

void Engine::Clear()
{
	for (std::shared_ptr<Level>& level : levels) level->Clear();
	levels.clear();
}

std::shared_ptr<Level> Engine::GetCurrentLevel() const
{
	return pCurrentLevel;
}

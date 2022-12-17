#include "MainEngine/Game.h"

#include <filesystem>
#include <regex>

#include "CoreEngine/Timer.h"
#include "CoreEngine/ThreadPool.h"
#include "GraphicsEngine/Graphics.h"
#include "GraphicsEngine/TextRenderer.h"
#include "GraphicsEngine/TextureFactory.h"
#include "SoundEngine/SoundSystem.h"
#include "ParserEngine/JSONManager.h"
#include "MainEngine/Engine.h"
#include "GameEngine/Levels/TitleLevel.h"
#include "GameEngine/Levels/SinglePlayerSelectionLevel.h"
#include "GameEngine/Levels/SinglePlayerCombatLevel.h"
#include "GameEngine/Levels/ConnectionLevel.h"
#include "GameEngine/Levels/MultiPlayerCombatLevel.h"
#include "GameEngine/Levels/MultiPlayerSelectionLevel.h"
#include "GameEngine/Levels/VictoryLevel.h"

Game::Game()
{
	// Graphics System Initialization
	GFX;

	// Textures Initialization
	for (const auto& entry : std::filesystem::recursive_directory_iterator("Textures")) {
		std::regex regex = std::regex("(.+/)*[1-9]?[0-9]\.tga");
		if (std::regex_search(entry.path().string(), regex)) ANIMATION_TEXTURE(entry.path().string());
		else if (entry.path().string().ends_with(".tga")) TEXTURE(entry.path().string());
	}

	// JSON Initialization
	for (const auto& entry : std::filesystem::recursive_directory_iterator("json"))
		if (entry.path().string().ends_with(".json")) JSON(entry.path().string());

	// Audio Initialization
	for (const auto& entry : std::filesystem::recursive_directory_iterator("Sounds"))
		if (entry.path().string().ends_with(".wav")) SFX.AddSong(entry.path().string(), true);

	// Levels Loading
	ENGINE.AddLevel<TitleLevel>();
	ENGINE.AddLevel<SinglePlayerSelectionLevel>();
	ENGINE.AddLevel<SinglePlayerCombatLevel>();
	ENGINE.AddLevel<ConnectionLevel>();
	ENGINE.AddLevel<MultiPlayerSelectionLevel>();
	ENGINE.AddLevel<MultiPlayerCombatLevel>();
	ENGINE.AddLevel<VictoryLevel>();
}

void Game::Go()
{
	GFX.BeginDraw();
	UpdateFrame();
	RenderFrame();
	GFX.EndDraw();
}

void Game::UpdateFrame() const
{
	ENGINE.Update();
}

void Game::RenderFrame() const
{
	ENGINE.Render();

	TEXT_ENGINE.Render(std::to_wstring(static_cast<int>(1 / DELTATIME)), L"Arial Black", 32.f, FRect(350.f, 300.f, 256.f, 64.f), D2D1::ColorF(D2D1::ColorF::Red));
}

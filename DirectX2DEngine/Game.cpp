#include "Game.h"

#include "ConnectionLevel.h"
#include "SinglePlayerCombatLevel.h"
#include "Engine.h"
#include "Graphics.h"
#include "MultiPlayerCombatLevel.h"
#include "MultiPlayerSelectionLevel.h"
#include "SinglePlayerSelectionLevel.h"
#include "SoundSystem.h"
#include "TextRenderer.h"
#include "ThreadPool.h"
#include "Timer.h"
#include "TitleLevel.h"

Game::Game()
{
	// Graphics System Initialization
	GFX;

	// Audio Initialization
	SFX.AddSong("Sounds/BeJustOrBeDead.wav", true);

	// Levels Loading
	ENGINE.AddLevel<TitleLevel>();
	ENGINE.AddLevel<SinglePlayerSelectionLevel>();
	ENGINE.AddLevel<SinglePlayerCombatLevel>();
	ENGINE.AddLevel<ConnectionLevel>();
	ENGINE.AddLevel<MultiPlayerSelectionLevel>();
	ENGINE.AddLevel<MultiPlayerCombatLevel>();
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

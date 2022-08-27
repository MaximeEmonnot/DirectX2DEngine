#include "Game.h"

#include "CombatLevel.h"
#include "Engine.h"
#include "Graphics.h"
#include "LoggerManager.h"
#include "Mouse.h"
#include "TextRenderer.h"
#include "Timer.h"

Game::Game()
{
	// Graphics System Initialization
	GFX;

	ENGINE.AddLevel<CombatLevel>();
}

void Game::Go()
{
	GFX.BeginDraw();
	UpdateFrame();
	RenderFrame();
	GFX.EndDraw();
}

void Game::UpdateFrame()
{
	ENGINE.Update();
}

void Game::RenderFrame()
{
	ENGINE.Render();

	TEXT_ENGINE.Render(std::to_wstring(static_cast<int>(1 / DELTATIME)), L"Arial Black", 64.f, FRect(300.f, 250.f, 256.f, 64.f), D2D1::ColorF(D2D1::ColorF::Red));
}

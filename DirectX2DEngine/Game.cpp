#include "Game.h"

#include "Engine.h"
#include "Graphics.h"

Game::Game()
{
	// Graphics System Initialization
	GFX;
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
}

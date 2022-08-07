#include "Game.h"

#include "Graphics.h"
#include "Timer.h"

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
	TICKCLOCK
}

void Game::RenderFrame()
{}

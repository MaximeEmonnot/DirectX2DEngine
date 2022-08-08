#include "Game.h"

#include "ColorModel.h"
#include "Graphics.h"
#include "Timer.h"

Game::Game()
	:
	test_model(std::make_shared<ColorModel>(0, 0, 750, 100, DirectX::XMFLOAT4(1.f, 1.f, 0.f, 1.f)))
{
	// Graphics System Initialization
	GFX;

	// Models Initialization
	test_model->Initialize();
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

	test_model->Update();
}

void Game::RenderFrame()
{
	test_model->Render();
}

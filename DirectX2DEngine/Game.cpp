#include "Game.h"

#include "ColorModel.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "TextureModel.h"
#include "Timer.h"

Game::Game()
	:
	test_model(std::make_shared<ColorModel>(0, 0, 750, 100, DirectX::XMFLOAT4(1.f, 1.f, 0.f, 1.f))),
	texture_model(std::make_shared<TextureModel>("Textures/China.tga"))
{
	// Graphics System Initialization
	GFX;

	// Models Initialization
	test_model->Initialize();
	texture_model->Initialize();
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

	//test_model->Update();
	texture_model->Update();
}

void Game::RenderFrame()
{
	//test_model->Render();
	texture_model->Render();
}

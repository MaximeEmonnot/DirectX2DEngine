#include "Game.h"

#include "Animation.h"
#include "ColorModel.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "LoggerManager.h"
#include "TextureModel.h"
#include "Timer.h"

Game::Game()
	:
	test_model(std::make_shared<ColorModel>(0, 0, 750, 100, DirectX::XMFLOAT4(1.f, 1.f, 0.f, 1.f))),
	texture_model(std::make_shared<TextureModel>())
{
	// Graphics System Initialization
	GFX;

	// Models Initialization
	test_model->Initialize();
	texture_model->Initialize();

	// Animation Initialization
	std::vector<std::string> frames;
	frames.emplace_back("Textures/China.tga");
	frames.emplace_back("Textures/Colony.tga");
	frames.emplace_back("Textures/London.tga");
	frames.emplace_back("Textures/Paris.tga");
	frames.emplace_back("Textures/PhantomCity.tga");
	frames.emplace_back("Textures/Zepp.tga");

	anim = std::make_shared<Animation>(frames, 0.5f, Animation::AnimationMode::LoopingAll);

	std::dynamic_pointer_cast<TextureModel>(texture_model)->SetTexture(anim->GetTexture());
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

	LOG(std::to_string(1 / DELTATIME), LOG_CONSOLE);
	LOG("Test Debug", LOG_DEBUG);
	LOG("Test Error", LOG_ERROR);

	//test_model->Update();
	texture_model->Update();
	anim->Update();
	std::dynamic_pointer_cast<TextureModel>(texture_model)->SetTexture(anim->GetTexture());
}

void Game::RenderFrame()
{
	//test_model->Render();
	texture_model->Render();
}

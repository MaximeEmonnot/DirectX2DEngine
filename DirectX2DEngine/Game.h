#pragma once
#include "Animation.h"
#include "BaseModel.h"

class Game
{
public:
	Game();
	~Game() = default;

	void Go();

private:
	void UpdateFrame();
	void RenderFrame();

private:
	std::shared_ptr<BaseModel> test_model;
	std::shared_ptr<BaseModel> texture_model;

	std::shared_ptr<Animation> anim;
};


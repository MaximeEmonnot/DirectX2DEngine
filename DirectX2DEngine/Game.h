#pragma once
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
};


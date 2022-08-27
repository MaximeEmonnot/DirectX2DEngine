#pragma once

class Game
{
public:
	Game();
	~Game() = default;

	void Go();

private:
	void UpdateFrame() const;
	void RenderFrame() const;
};


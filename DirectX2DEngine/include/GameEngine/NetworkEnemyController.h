#pragma once
#include "MainEngine/Controller.h"
#include "MathEngine/Vec2D.h"

class NetworkEnemyController :
    public Controller
{
public:
	NetworkEnemyController(Pawn& owner);

	void Update() override;
private:
    FVec2D lastPosition;
};


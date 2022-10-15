#pragma once

#include "MathEngine/Vec2D.h"
#include "MainEngine/Controller.h"

class NetworkEnemyController :
    public Controller
{
public:
	NetworkEnemyController(Pawn& owner);

	void Update() override;
private:
    FVec2D lastPosition;
};


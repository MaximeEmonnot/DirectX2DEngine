#pragma once

#include "FightingController.h"

#include "MathEngine/Vec2D.h"

class NetworkEnemyController :
    public FightingController
{
public:
	NetworkEnemyController(Pawn& owner);

	void	Update() override;
private:
    FVec2D	lastPosition;
};


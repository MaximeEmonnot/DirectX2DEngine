#pragma once
#include "Controller.h"
#include "Vec2D.h"

class NetworkEnemyController :
    public Controller
{
public:
	NetworkEnemyController(Pawn& owner);

	void Update() override;
private:
    FVec2D lastPosition;
};


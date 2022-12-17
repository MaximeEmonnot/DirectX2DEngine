#include "GameEngine\Controllers\FightingController.h"

FightingController::FightingController(Pawn& owner)
	:
	Controller(owner)
{}

void FightingController::SetLookAtDirection(bool bValue)
{
	bIsLookingLeft = bValue;
}

bool FightingController::IsDoingAction(const EAction & actionTest) const
{
	return action == actionTest;
}

#include "GameEngine/Controllers/DummyController.h"

DummyController::DummyController(Pawn& owner)
	:
	FightingController(owner)
{}

void DummyController::Update()
{}

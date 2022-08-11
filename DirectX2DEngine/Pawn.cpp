#include "Pawn.h"

Pawn::Pawn(const FVec2D& pos, const std::string& name, std::shared_ptr<Controller> pController)
	:
	Actor(pos, name),
	pController(pController)
{
}

Pawn& Pawn::operator=(const Pawn& toCopy)
{
	pos = toCopy.pos;
	name = toCopy.name;
	pController = toCopy.pController;
	return *this;
}

void Pawn::Update()
{
	Actor::Update();

	pController->Update();
}

void Pawn::Render()
{
}

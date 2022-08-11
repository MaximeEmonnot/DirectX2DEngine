#include "Character.h"

Character::Character(const FVec2D& pos, const std::string& name, std::shared_ptr<Controller> pController)
	:
	Pawn(pos, name, pController)
{
}

void Character::Update()
{
}

void Character::Render()
{
}

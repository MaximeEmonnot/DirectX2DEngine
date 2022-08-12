#include "Character.h"

#include "Collider.h"

Character::Character(const FVec2D& pos, const std::string& name, std::shared_ptr<Controller> pController)
	:
	Pawn(pos, name, pController),
	rootCollider(std::make_shared<Collider>(*this))
{
}

void Character::Update()
{
	Pawn::Update();
	rootCollider->Update();
}

void Character::Render()
{
	Pawn::Render();
	rootCollider->Render();
}

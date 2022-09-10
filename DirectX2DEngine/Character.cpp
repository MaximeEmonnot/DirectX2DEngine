#include "Character.h"

#include "Collider.h"

Character::Character(World& world, const FVec2D& pos, const std::string& name, std::shared_ptr<Controller> pController)
	:
	Pawn(world, pos, name, pController),
	rootCollider(std::make_shared<Collider>(*this))
{
}

void Character::Update()
{
	Pawn::Update();
	rootCollider->Update();
}

std::shared_ptr<Collider> Character::GetRootCollider() const
{
	return rootCollider;
}

bool Character::IsGrounded() const
{
	return !rootCollider->IsFalling();
}

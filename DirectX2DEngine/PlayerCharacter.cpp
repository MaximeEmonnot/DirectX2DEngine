#include "PlayerCharacter.h"

#include "Collider.h"
#include "PlayerController.h"
#include "Rect.h"
#include "RoboKy.h"
#include "SolBadguy.h"

PlayerCharacter::PlayerCharacter(World& world, const FVec2D& pos, const std::string& name)
	:
	Character(world, pos, name, std::make_shared<PlayerController>(*this)),
	pFighter(std::make_shared<SolBadguy>(*this))
{
	rootCollider->SetRectPos(FRect(0, 5, 10, 10));
	rootCollider->SetCollisionMode(Collider::CollisionMode::Blocking);
	rootCollider->SetGravity(true);
	rootCollider->SetVisible(true);
	rootCollider->SetCollisionChannel(Collider::CollisionChannel::Gravity);
}

void PlayerCharacter::Update()
{
	Character::Update();

	pFighter->Update();
}

std::vector<std::shared_ptr<Collider>> PlayerCharacter::GetColliders() const 
{
	std::vector<std::shared_ptr<Collider>> out;
	out.emplace_back(rootCollider);
	return out;
}

bool PlayerCharacter::IsGrounded() const
{
	return !rootCollider->IsFalling();
}

std::shared_ptr<BaseFighter> PlayerCharacter::GetFighter()
{
	return pFighter;
}

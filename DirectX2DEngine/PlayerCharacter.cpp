#include "PlayerCharacter.h"

#include "Collider.h"
#include "PlayerController.h"
#include "Rect.h"
#include "SolBadguy.h"

PlayerCharacter::PlayerCharacter(const FVec2D& pos, const std::string& name)
	:
	Character(pos, name, std::make_shared<PlayerController>(*this)),
	pFighter(std::make_unique<SolBadguy>(*this))
{
	rootCollider->SetRectPos(FRect(0, 5, 10, 10));
	rootCollider->SetCollisionMode(Collider::CollisionMode::Blocking);
	rootCollider->SetGravity(true);
	rootCollider->SetCollisionChannel(Collider::CollisionChannel::Gravity);
	rootCollider->SetVisible(true);
}

void PlayerCharacter::Update()
{
	Character::Update();

	pFighter->Update();
}

void PlayerCharacter::Render()
{
	Character::Render();

	pFighter->Render();
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
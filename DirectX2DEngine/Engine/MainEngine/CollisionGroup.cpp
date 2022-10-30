#include "MainEngine/CollisionGroup.h"

void CollisionGroup::Update()
{
	for (auto& collider : colliders) collider.Update();
}

void CollisionGroup::Reset()
{
	for (auto& collider : colliders) collider.Reset();
}

void CollisionGroup::AddCollider(const MovingCollider& collider)
{
	colliders.emplace_back(collider);
}

bool CollisionGroup::IsFinished() const
{
	bool out = false;
	for (auto& col : colliders) out |= col.IsFinished();
	return out;
}

std::vector<std::shared_ptr<Collider>> CollisionGroup::GetColliders() const
{
	std::vector<std::shared_ptr<Collider>> out;
	for (auto& movingCollider : colliders) out.emplace_back(movingCollider.GetCollider());
	return out;
}

void CollisionGroup::SetDirection(int newDirection) const
{
	for (auto& movingCollider : colliders) movingCollider.SetDirection(newDirection);
}

void CollisionGroup::SetVisible(bool bValue) const
{
	for (auto& movingCollider : colliders) movingCollider.SetVisible(bValue);
}

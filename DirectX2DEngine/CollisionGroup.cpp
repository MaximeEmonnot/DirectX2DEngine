#include "CollisionGroup.h"

void CollisionGroup::Update()
{
	for (auto& collider : colliders) collider.Update();
}

void CollisionGroup::Render()
{
	for (auto& collider : colliders) collider.Render();
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
	for (auto& c : colliders) out.emplace_back(c.GetCollider());
	return out;
}

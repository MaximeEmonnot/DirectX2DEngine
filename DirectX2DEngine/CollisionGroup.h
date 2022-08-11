#pragma once
#include "MovingCollider.h"
class CollisionGroup
{
public:
	CollisionGroup() = default;

	void Update();
	void Render();

	void Reset();

	void AddCollider(const MovingCollider& collider);

	bool IsFinished() const;

	std::vector<std::shared_ptr<Collider>> GetColliders() const;

private:
	std::vector<MovingCollider> colliders;
};


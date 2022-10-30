#pragma once

#include "MovingCollider.h"

class CollisionGroup
{
public:
	CollisionGroup() = default;

	void									Update();
											
	void									Reset();
											
	void									AddCollider(const MovingCollider& collider);
											
	bool									IsFinished() const;

	std::vector<std::shared_ptr<Collider>>	GetColliders() const;

	void									SetDirection(int direction) const;
	void									SetVisible(bool bValue) const;

private:
	std::vector<MovingCollider>				colliders;
};


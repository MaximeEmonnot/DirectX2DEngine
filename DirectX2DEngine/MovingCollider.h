#pragma once
#include "Collider.h"

class MovingCollider
{
public:
	MovingCollider(Actor& owner);

	void Update();
	void Render();

	void SetLoop(bool bValue);
	void SetHoldTime(float _holdTime);
	void SetCollisionMode(Collider::CollisionMode cMode);
	void SetCollisionChannel(Collider::CollisionChannel cChannel);
	void SetVisible(bool bValue);
	void AddPosition(const FRect& pos);

	void Reset();

	bool IsFinished() const;

	std::shared_ptr<Collider> GetCollider() const;

private:
	void Advance();

private:
	FRect defaultPos = FRect(0.0, 0.0, 0.0, 0.0);

	std::shared_ptr<Collider> collider;
	std::vector<FRect> positions;
	float curFrameTime = 0.0f;
	float holdTime;
	int curRectPos = 0;

	bool bIsLooping = true;
	bool bIsFinished = false;
};


#pragma once
#include <vector>
#include <unordered_set>
#include <memory>
#include "Rect.h"
#include "Actor.h"
#include "ColorModel.h"
class Collider
{
private:
	friend class MovingCollider;
public:
	enum class CollisionMode
	{
		None,
		Overlapping,
		Invicibility,
		Blocking
	};

	enum class CollisionChannel
	{
		Stun,
		Defense,
		Attack,
		Gravity,
		None
	};
public:
	Collider(Actor& owner);
	Collider(Actor& owner, const FRect& rect);

	Collider& operator=(const Collider& collider);

	void Update();
	void Render();

	void SetCollisionMode(CollisionMode cMode);
	void SetVisible(bool bValue);
	void SetCollisionChannel(CollisionChannel cChannel);
	void SetGravity(bool bValue);
	void SetRectPos(const FRect& pos);

	bool IsOverlapping() const;
	bool IsBlocking() const;

	bool IsFalling() const;

	std::unordered_set<std::shared_ptr<Actor>, Actor::Hash> GetOverlappingActors() const;

	void AddForce(const FVec2D& dir);
	void AddImpulse(const FVec2D& dir);
	void AddInputMovement(const FVec2D& dir);


private:
	void ApplyGravity();
	void ApplyForces();
	void ApplyFriction();
	void ApplyReaction(const FRect& test_rect, FVec2D& direction);

	void UpdateInvincibility();
	void TryMovingInThisDirection(FVec2D& direction);
	void SetInvicibilityState();

private:
	bool bIsBlocking = false;
	bool bIsOverlapping = false;
	bool bIsVisible = false;
	bool bIsGravityEnabled = false;

	CollisionMode mode = CollisionMode::None;
	CollisionChannel channel = CollisionChannel::None;

	ColorModel model;

	Actor& owner;
	FVec2D& origin;
	FRect rect;

	FVec2D velocity;

	std::unordered_set<std::shared_ptr<Actor>, Actor::Hash> overlappingActors;

	std::vector<FVec2D> forces;

	static constexpr FVec2D topNormal = FVec2D(0, 1);
	static constexpr FVec2D rightNormal = FVec2D(1, 0);

	static constexpr FVec2D gravity = FVec2D(0, -10);
	float gravitySpeed = 1.0f;

	int nInvicibilityFrames = 0;
};


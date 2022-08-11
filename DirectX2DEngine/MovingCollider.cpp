#include "MovingCollider.h"
#include "Timer.h"

MovingCollider::MovingCollider(Actor& owner)
	:
	collider(std::make_shared<Collider>(owner, defaultPos))
{
	collider->SetCollisionMode(Collider::CollisionMode::None);
}

void MovingCollider::Update()
{
	collider->SetRectPos(positions[curRectPos]);
	collider->Update();
	curFrameTime += DELTATIME;
	while (curFrameTime > holdTime) Advance();
}

void MovingCollider::Render()
{
	collider->Render();
}

void MovingCollider::SetLoop(bool bValue)
{
	bIsLooping = bValue;
}

void MovingCollider::SetHoldTime(float _holdTime)
{
	holdTime = _holdTime;
}

void MovingCollider::SetCollisionMode(Collider::CollisionMode cMode)
{
	collider->SetCollisionMode(cMode);
}

void MovingCollider::SetCollisionChannel(Collider::CollisionChannel cChannel)
{
	collider->SetCollisionChannel(cChannel);
}

void MovingCollider::SetVisible(bool bValue)
{
	collider->SetVisible(bValue);
}

void MovingCollider::AddPosition(const FRect& pos)
{
	positions.emplace_back(pos);
}

void MovingCollider::Reset()
{
	bIsFinished = false;
	curRectPos = 0;
}

bool MovingCollider::IsFinished() const
{
	return bIsFinished;
}

std::shared_ptr<Collider> MovingCollider::GetCollider() const
{
	return collider;
}

void MovingCollider::Advance()
{
	if (++curRectPos >= positions.size()) {
		curRectPos = (bIsLooping) ? 0 : static_cast<int>(positions.size() - 1);
		bIsFinished = !bIsLooping;
	}
	curFrameTime -= holdTime;
}

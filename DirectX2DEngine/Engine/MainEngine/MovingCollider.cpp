#include "MainEngine/MovingCollider.h"

#include "CoreEngine/Timer.h"

MovingCollider::MovingCollider(Actor& owner)
	:
	pCollider(std::make_shared<Collider>(owner, defaultPos))
{
	pCollider->SetCollisionMode(Collider::CollisionMode::None);
}

void MovingCollider::Update()
{
	pCollider->SetRectPos(positions[curRectPos]);
	pCollider->Update();
	curFrameTime += DELTATIME;
	while (curFrameTime > holdTime) Advance();
}

void MovingCollider::SetLoop(Animation::AnimationMode _loopMode)
{
	loopMode = _loopMode;
}

void MovingCollider::SetHoldTime(float _holdTime)
{
	holdTime = _holdTime;
}

void MovingCollider::SetCollisionMode(Collider::CollisionMode collisionMode)
{
	pCollider->SetCollisionMode(collisionMode);
}

void MovingCollider::SetCollisionChannel(Collider::CollisionChannel collisionChannel)
{
	pCollider->SetCollisionChannel(collisionChannel);
}

void MovingCollider::SetVisible(bool bValue)
{
	pCollider->SetVisible(bValue);
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
	return pCollider;
}

void MovingCollider::Advance()
{
	// We go into the next frame, depending on the AnimationLoopingMode
	curRectPos += threshold;
	if (curRectPos >= positions.size() || curRectPos < 0) {
		switch (loopMode)
		{
		case Animation::AnimationMode::NoLoop:
			curRectPos = static_cast<int>(positions.size() - 1);
			bIsFinished = true;
			break;
		case Animation::AnimationMode::LoopingAll:
			curRectPos = 0;
			break;
		case Animation::AnimationMode::LoopingBack:
			bIsFinished = (curRectPos < 0);
			threshold *= -1;
			curRectPos += 2 * threshold;
			break;
		default:
			break;
		}
	}
	else bIsFinished = false;
	curFrameTime -= holdTime;
}

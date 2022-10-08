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

void MovingCollider::SetLoop(Animation::AnimationMode loop_mode)
{
	loopMode = loop_mode;
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

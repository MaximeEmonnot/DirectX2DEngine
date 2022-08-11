#include "Actor.h"

Actor::Actor(const FVec2D& pos, const std::string& name)
	:
	pos(pos),
	name(name)
{
}

Actor& Actor::operator=(const Actor& toCopy)
{
	pos = toCopy.pos;
	name = toCopy.name;
	return *this;
}

void Actor::Update()
{
}

void Actor::Render()
{
}

FVec2D Actor::GetPosition() const
{
	return pos;
}

std::string Actor::GetName() const
{
	return name;
}

std::vector<std::shared_ptr<class Collider>> Actor::GetColliders() const
{
	return std::vector<std::shared_ptr<class Collider>>();
}

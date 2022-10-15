#include "MainEngine/Actor.h"
#include "MainEngine/World.h"

Actor::Actor(World& world, const FVec2D& pos, const std::string& name)
	:
	world(world),
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

FVec2D Actor::GetPosition() const
{
	return pos;
}

void Actor::SetPosition(const FVec2D& new_position)
{
	pos = new_position;
}

std::string Actor::GetName() const
{
	return name;
}

std::vector<std::shared_ptr<class Collider>> Actor::GetColliders() const
{
	return std::vector<std::shared_ptr<class Collider>>();
}


World& Actor::GetWorld() const
{
	return world;
}

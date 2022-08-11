#include "Actor.h"

Actor::Actor(const DirectX::XMFLOAT2& pos, const std::string& name)
	:
	pos(pos),
	name(name)
{
}

Actor& Actor::operator=(const Actor& toCopy)
{
	pos = toCopy.pos;
	name = toCopy.name;
}

void Actor::Update()
{
}

void Actor::Render()
{
}

DirectX::XMFLOAT2 Actor::GetPosition() const
{
	return pos;
}

std::string Actor::GetName() const
{
	return name;
}

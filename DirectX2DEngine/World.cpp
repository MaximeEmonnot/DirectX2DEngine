#include "World.h"

void World::Update()
{
	for (std::shared_ptr<Actor>& actor : actors)
		actor->Update();
}

std::vector<std::shared_ptr<Actor>> World::GetActors() const
{
	return actors;
}

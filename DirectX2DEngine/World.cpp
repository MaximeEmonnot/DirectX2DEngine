#include "World.h"

void World::Update()
{
	for (std::shared_ptr<Actor>& actor : actors)
		actor->Update();
}

void World::Render() const
{
	for (const std::shared_ptr<Actor>& actor : actors)
		actor->Render();
}

std::vector<std::shared_ptr<Actor>> World::GetActors() const
{
	return actors;
}

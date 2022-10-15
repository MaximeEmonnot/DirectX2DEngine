#include "MainEngine/World.h"

#include "MainEngine/Actor.h"

void World::Update()
{
	for (std::shared_ptr<Actor>& actor : actors)
		actor->Update();
}

void World::Render() const
{
	for (const std::pair<const int, std::shared_ptr<BaseModel>>& entry : models)
		if (entry.second->IsVisible()) entry.second->Render();
}

void World::Clear()
{
	actors.clear();
	models.clear();
}

std::vector<std::shared_ptr<Actor>> World::GetActors() const
{
	return actors;
}

#include "Level.h"

Level::Level()
	:
	pWorld(std::make_shared<World>())
{
}

Level::Level(const Level& toCopy)
{
	*this = toCopy;
}

Level::Level(Level&& toMove) noexcept
	:
	pWorld(std::move(toMove.pWorld))
{
}

Level& Level::operator=(const Level& toCopy)
{
	pWorld = toCopy.pWorld;
	return *this;
}

Level& Level::operator=(Level&& toMove) noexcept
{
	pWorld = std::move(toMove.pWorld);
	return *this;
}

void Level::Update()
{
	pWorld->Update();
	for (const std::shared_ptr<AUICanvas>& canvas : canvasList) canvas->Update();
}

void Level::Render() const
{
	pWorld->Render();

	for (const std::shared_ptr<AUICanvas>& canvas : canvasList) canvas->Render();
}

World& Level::GetWorld() const
{
	return *pWorld;
}

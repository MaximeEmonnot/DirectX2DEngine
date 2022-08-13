#pragma once
#include <memory>

#include "World.h"

class Level
{
public:
	Level();
	Level(const Level& toCopy);
	Level(Level&& toMove) noexcept;

	Level& operator= (const Level& toCopy);
	Level& operator= (Level&& toMove) noexcept;

	virtual ~Level() = default;

	virtual void Update();

	World& GetWorld() const;

protected:
	std::shared_ptr<World> pWorld;
};

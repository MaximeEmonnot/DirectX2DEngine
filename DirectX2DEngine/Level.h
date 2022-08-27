#pragma once
#include <map>
#include <memory>

#include "BaseModel.h"
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

	void Render() const;

	World& GetWorld() const;

protected:
	std::shared_ptr<World> pWorld;
};

#pragma once

#include <memory>

#include "UIEngine/AUICanvas.h"
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

	virtual void BeginLevel() = 0;

	void Render() const;

	void Clear();

	World& GetWorld() const;

protected:
	template<class T>
	void CreateCanvas()
	{
		canvasList.emplace_back(std::make_shared<T>());
	}

	template<class T, class... Args>
	void CreateCanvas(Args&&... args)
	{
		canvasList.emplace_back(std::make_shared<T>(args...));
	}

protected:
	std::shared_ptr<World> pWorld;
	std::vector<std::shared_ptr<AUICanvas>> canvasList;
};

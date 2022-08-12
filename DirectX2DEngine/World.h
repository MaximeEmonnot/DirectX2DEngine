#pragma once
#include "Actor.h"
#include <memory>
#include <vector>

class World
{
public:
	World() = default;
	 
	void Update();
	void Render() const;

	template<class T>
	void SpawnActor()
	{
		actors.emplace_back(std::make_shared<T>());
	}

	template<class T, class... Args >
	void SpawnActor(Args&&... args)
	{
		actors.emplace_back(std::make_shared<T>(args...));
	}

	std::vector<std::shared_ptr<Actor>> GetActors() const;

private:
	std::vector<std::shared_ptr<Actor>> actors;
};


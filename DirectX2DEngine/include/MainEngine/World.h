#pragma once

#include <map>
#include <memory>
#include <vector>

#include "GraphicsEngine/BaseModel.h"

class World
{
public:
	World() = default;
	 
	void Update();
	void Render() const;

	void Clear();

	template<class T>
	std::shared_ptr<T> SpawnActor()
	{
		std::shared_ptr<T> new_actor = std::make_shared<T>(*this);
		actors.emplace_back(new_actor);
		return new_actor;
	}

	template<class T, class... Args >
	std::shared_ptr<T> SpawnActor(Args&&... args)
	{
		std::shared_ptr<T> new_actor = std::make_shared<T>(*this, args...);
		actors.emplace_back(new_actor);
		return new_actor;
	}

	template<class T>
	std::shared_ptr<T> CreateModel(int priority)
	{
		std::shared_ptr<BaseModel> new_model = std::make_shared<T>();
		new_model->Initialize();
		new_model->SetDepth(static_cast<float>(priority));
		models.emplace(std::pair(priority, new_model));
		return std::dynamic_pointer_cast<T>(new_model);
	}

	template<class T, class... Args>
	std::shared_ptr<T> CreateModel(int priority, Args&&... args)
	{
		std::shared_ptr<BaseModel> new_model = std::make_shared<T>(args...);
		new_model->Initialize();
		new_model->SetDepth(static_cast<float>(priority));
		models.emplace(std::pair(priority, new_model));
		return std::dynamic_pointer_cast<T>(new_model);
	}

	std::vector<std::shared_ptr<class Actor>> GetActors() const;

private:
	std::vector<std::shared_ptr<class Actor>> actors;

	std::multimap<int, std::shared_ptr<BaseModel>> models;
};


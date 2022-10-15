#pragma once
#include <memory>
#include <vector>
#include <string>
#include "MathEngine/Vec2D.h"
#include "World.h"

class Actor
{
public:
	class Hash
	{
	public:
		size_t operator()(const Actor& a) const {
			constexpr std::hash<std::string> hasher;
			return hasher(a.name);
		}
		size_t operator()(const std::shared_ptr<Actor>& a) const
		{
			constexpr std::hash<std::string> hasher;
			return hasher(a->name);
		}
	};

private:
	friend class Controller;
	friend class Collider;
	friend class World;
public:
	Actor() = delete;
	Actor(World& world, const FVec2D& pos, const std::string& name);
	Actor(const Actor&) = default;
	virtual ~Actor() = default;

	Actor& operator=(const Actor& toCopy);

	virtual void Update();

	FVec2D GetPosition() const;
	void SetPosition(const FVec2D& new_position);
	std::string GetName() const;
	virtual std::vector<std::shared_ptr<class Collider>> GetColliders() const;

	World& GetWorld() const;

protected:
	World& world;
	FVec2D pos;
	std::string name;
 };


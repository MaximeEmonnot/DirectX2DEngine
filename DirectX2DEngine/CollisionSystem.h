#pragma once
#include "CollisionGroup.h"
class CollisionSystem
{
public:
	CollisionSystem(Actor& owner, const std::string& file_path, const std::vector<std::string>& animations);

	void Update();

	void AddTransition(const std::string& from, const std::string& to, const std::function<bool()>& condition);

	bool AnimationIsFinished() const;

	std::vector<std::shared_ptr<Collider>> GetColliders() const;

private:
	void SetCollisionGroup(const std::string& state);

private:
	std::string animState;
	std::unordered_map<std::string, CollisionGroup> collisionGroups;
	std::unordered_multimap<std::string, std::pair<std::string, std::function<bool()>>> transitions;
};


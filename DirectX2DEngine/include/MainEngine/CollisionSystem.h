#pragma once
#include "CollisionGroup.h"
#include "rapidjson/document.h"

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
	void AddCollisionsToGroup(CollisionGroup& collision_group, Actor& owner, rapidjson::GenericValue<rapidjson::UTF8<>>& jsonObject, const std::string& name, Collider::CollisionChannel collision_channel, float hold_time, Animation::AnimationMode loop_mode, const std::string& animation) const;

private:
	std::string animState;
	std::unordered_map<std::string, CollisionGroup> collisionGroups;
	std::unordered_multimap<std::string, std::pair<std::string, std::function<bool()>>> transitions;
};


#include "CollisionSystem.h"
#include "JSONParser.h"
#include "TextureFactory.h"

CollisionSystem::CollisionSystem(Actor& owner, const std::string& file_path, const std::vector<std::string>& animations)
{
	JSONParser::Reader jsonParser;
	jsonParser.ReadFile(file_path);

	auto& jsonCollisions = jsonParser.GetValueOf("collisions");
	for (auto& anim : animations)
	{
		if (jsonCollisions.HasMember(anim.c_str()))
		{
			CollisionGroup collision_group;
			auto& jsonCollider = jsonCollisions[anim.c_str()];

			const float hold_time = jsonParser.GetValueOf("animations")[anim.c_str()].GetArray()[1].GetFloat();
			const Animation::AnimationMode loop_mode = static_cast<Animation::AnimationMode>(jsonParser.GetValueOf("animations")[anim.c_str()].GetArray()[2].GetInt());
			const std::string animation_path = jsonParser.GetValueOf("character").GetString() + anim + std::string("/");

			AddCollisionsToGroup(collision_group, owner, jsonCollider, "Head", Collider::CollisionChannel::Stun, hold_time, loop_mode, animation_path);
			AddCollisionsToGroup(collision_group, owner, jsonCollider, "Body", Collider::CollisionChannel::Defense, hold_time, loop_mode, animation_path);
			AddCollisionsToGroup(collision_group, owner, jsonCollider, "Attack", Collider::CollisionChannel::Attack, hold_time, loop_mode, animation_path);

			collisionGroups.insert(std::pair(anim, collision_group));
		}
	}

	animState = "Idle";
}

void CollisionSystem::Update()
{
	collisionGroups.at(animState).Update();
	for (auto& entry : transitions) if (entry.first == animState && entry.second.second()) SetCollisionGroup(entry.second.first);
}

void CollisionSystem::AddTransition(const std::string& from, const std::string& to, const std::function<bool()>& condition)
{
	transitions.insert(std::pair(from, std::pair(to, condition)));
}

bool CollisionSystem::AnimationIsFinished() const
{
	return collisionGroups.at(animState).IsFinished();
}

std::vector<std::shared_ptr<Collider>> CollisionSystem::GetColliders() const
{
	std::vector<std::shared_ptr<Collider>> out;
	for (auto& c : collisionGroups.at(animState).GetColliders())
		out.emplace_back(c);
	return out;
}

void CollisionSystem::SetCollisionGroup(const std::string& state)
{
	if (animState != state) {
		animState = state;
		collisionGroups.at(animState).Reset();
	}
}

void CollisionSystem::AddCollisionsToGroup(CollisionGroup& collision_group, Actor& owner, rapidjson::GenericValue<rapidjson::UTF8<>>& jsonObject, const std::string& name, Collider::CollisionChannel collision_channel, float hold_time, Animation::AnimationMode loop_mode, const std::string& animation) const
{
	if (jsonObject.HasMember(name.c_str()))
	{
		auto& collider = jsonObject[name.c_str()];
		MovingCollider moving_collider(owner);
		moving_collider.SetVisible(true);
		moving_collider.SetCollisionMode(Collider::CollisionMode::Overlapping);
		moving_collider.SetCollisionChannel(collision_channel);
		moving_collider.SetHoldTime(hold_time);
		moving_collider.SetLoop(loop_mode);
		for (auto itr = collider.MemberBegin(); itr != collider.MemberEnd(); ++itr)
		{
			const float x_pos = itr->value.GetArray()[0].GetFloat();
			const float y_pos = itr->value.GetArray()[1].GetFloat();
			const float width = itr->value.GetArray()[2].GetFloat();
			const float height = itr->value.GetArray()[3].GetFloat();
			const FVec2D texture_center = ANIMATION_TEXTURE(animation + std::string(itr->name.GetString()) + std::string(".tga")).GetCenter();
			moving_collider.AddPosition(FRect(x_pos - texture_center.x, y_pos - texture_center.y, width, height));
		}
		collision_group.AddCollider(moving_collider);
	}
}

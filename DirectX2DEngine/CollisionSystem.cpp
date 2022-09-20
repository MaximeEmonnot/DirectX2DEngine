#include "CollisionSystem.h"
#include "JSONParser.h"

CollisionSystem::CollisionSystem(Actor& owner, const std::string& file_path, const std::vector<std::string>& animations)
{
	JSONParser::Reader jsonParser;
	jsonParser.ReadFile(file_path);

	auto& v = jsonParser.GetValueOf("collisions");
	for (auto& anim : animations)
	{
		if (v.HasMember(anim.c_str()))
		{
			CollisionGroup collision_group;
			auto& m = v[anim.c_str()];

			const int width = jsonParser.GetValueOf("animations")[anim.c_str()].GetArray()[2].GetInt();
			const int height = jsonParser.GetValueOf("animations")[anim.c_str()].GetArray()[3].GetInt();

			const float xRatio = static_cast<float>(width) / static_cast<float>(width);
			const float yRatio = static_cast<float>(height) / static_cast<float>(height);

			if (m.HasMember("Body"))
			{
				auto& c = m["Body"];
				MovingCollider moving_collider(owner);
				moving_collider.SetVisible(true);
				moving_collider.SetCollisionMode(Collider::CollisionMode::Overlapping);
				moving_collider.SetCollisionChannel(Collider::CollisionChannel::Defense);
				moving_collider.SetHoldTime(m["HoldTime"].GetFloat());
				moving_collider.SetLoop(m["IsLooping"].GetBool());
				for (auto itr = c.MemberBegin(); itr != c.MemberEnd(); ++itr)
				{
					const float x_pos = itr->value.GetArray()[0].GetFloat();
					const float y_pos = itr->value.GetArray()[1].GetFloat();
					const float coll_width = itr->value.GetArray()[2].GetFloat();
					const float coll_height = itr->value.GetArray()[3].GetFloat();
					moving_collider.AddPosition(FRect(xRatio * (x_pos - (width - coll_width) / 2), yRatio * (-y_pos + (height - coll_height) / 2), xRatio * coll_width, yRatio * coll_height));
				}
				collision_group.AddCollider(moving_collider);
			}
			if (m.HasMember("Head"))
			{
				auto& c = m["Head"];
				MovingCollider moving_collider(owner);
				moving_collider.SetVisible(true);
				moving_collider.SetCollisionMode(Collider::CollisionMode::Overlapping);
				moving_collider.SetCollisionChannel(Collider::CollisionChannel::Stun);
				moving_collider.SetHoldTime(m["HoldTime"].GetFloat());
				moving_collider.SetLoop(m["IsLooping"].GetBool());
				for (auto itr = c.MemberBegin(); itr != c.MemberEnd(); ++itr)
				{
					const float x_pos = itr->value.GetArray()[0].GetFloat();
					const float y_pos = itr->value.GetArray()[1].GetFloat();
					const float coll_width = itr->value.GetArray()[2].GetFloat();
					const float coll_height = itr->value.GetArray()[3].GetFloat();
					moving_collider.AddPosition(FRect(xRatio * (x_pos - (width - coll_width) / 2), yRatio * (-y_pos + (height - coll_height) / 2), xRatio * coll_width, yRatio * coll_height));
				}
				collision_group.AddCollider(moving_collider);
			}
			if (m.HasMember("Attack"))
			{
				auto& c = m["Attack"];
				MovingCollider moving_collider(owner);
				moving_collider.SetVisible(true);
				moving_collider.SetCollisionMode(Collider::CollisionMode::Overlapping);
				moving_collider.SetCollisionChannel(Collider::CollisionChannel::Attack);
				moving_collider.SetHoldTime(m["HoldTime"].GetFloat());
				moving_collider.SetLoop(m["IsLooping"].GetBool());
				for (auto itr = c.MemberBegin(); itr != c.MemberEnd(); ++itr)
				{
					const float x_pos = itr->value.GetArray()[0].GetFloat();
					const float y_pos = itr->value.GetArray()[1].GetFloat();
					const float coll_width = itr->value.GetArray()[2].GetFloat();
					const float coll_height = itr->value.GetArray()[3].GetFloat();
					moving_collider.AddPosition(FRect(xRatio * (x_pos - (width - coll_width) / 2), yRatio * (-y_pos + (height - coll_height) / 2), xRatio * coll_width, yRatio * coll_height));
				}
				collision_group.AddCollider(moving_collider);
			}
			collisionGroups.insert(std::pair(anim, collision_group));
		}
	}

	animState = "IdleRight";
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

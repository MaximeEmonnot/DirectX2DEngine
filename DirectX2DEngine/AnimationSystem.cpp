#include "AnimationSystem.h"
#include "JSONParser.h"

AnimationSystem::AnimationSystem(const std::string& filePath)
{
	JSONParser::Reader jsonParser;
	jsonParser.ReadFile(filePath);

	const std::string character = jsonParser.GetValueOf("character").GetString();
	auto& v = jsonParser.GetValueOf("animations");
	for (auto itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr)
	{
		const std::string anim_name = itr->name.GetString();
		std::vector<std::string> sprites;
		const int count = itr->value.GetArray()[0].GetInt();
		const float holdTime = itr->value.GetArray()[1].GetFloat();
		const Animation::AnimationMode animation_mode = static_cast<Animation::AnimationMode>(itr->value.GetArray()[2].GetInt());

		for (int i = 0; i < count; i++) sprites.push_back(character + anim_name + std::string("/") + std::to_string(i) + std::string(".tga"));

		animations.insert(std::pair(anim_name, Animation(sprites, holdTime, animation_mode)));

		if (animState.empty()) animState = anim_name;
	}
}

void AnimationSystem::Update()
{
	animations.at(animState).Update();
	for (auto& entry : transitions) if (entry.first == animState && entry.second.second()) SetAnimation(entry.second.first);
}

void AnimationSystem::SetAnimation(const std::string& animName)
{
	if (animState != animName) {
		animState = animName;
		animations.at(animState).Reset();
	}
}

void AnimationSystem::AddTransition(const std::string& from, const std::string& to, const std::function<bool()>& condition)
{
	transitions.insert(std::pair<std::string, std::pair<std::string, std::function<bool()>>>(from, std::pair<std::string, std::function<bool()>>(to, condition)));
}

Texture AnimationSystem::GetTexture() const
{
	return animations.at(animState).GetTexture();
}

std::string AnimationSystem::GetCurrentAnimation() const
{
	return animState;
}

std::vector<std::string> AnimationSystem::GetAnimationList()
{
	std::vector<std::string> out;
	for (auto& entry : animations) out.emplace_back(entry.first);
	return out;
}

bool AnimationSystem::AnimationIsFinished() const
{
	return animations.at(animState).IsFinished();
}

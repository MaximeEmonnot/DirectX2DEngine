#pragma once
#include "Animation.h"
#include <unordered_map>
#include <string>

#include "Vec2D.h"

class AnimationSystem
{
public:
	AnimationSystem(const std::string& filePath);

	void Update();

	void SetAnimation(const std::string& animName);
	void AddTransition(const std::string& from, const std::string& to, const std::function<bool()>& condition);

	Texture GetTexture() const;

	std::string GetCurrentAnimation() const;

	std::vector<std::string> GetAnimationList();

	bool AnimationIsFinished() const;
private:
	std::string animState = "";
	std::unordered_map<std::string, Animation> animations;
	std::unordered_multimap<std::string, std::pair<std::string, std::function<bool()>>> transitions;
};


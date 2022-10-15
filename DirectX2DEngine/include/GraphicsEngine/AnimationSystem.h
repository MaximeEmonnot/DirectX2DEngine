#pragma once

#include <unordered_map>
#include <string>

#include "Animation.h"
#include "MathEngine/Vec2D.h"

class AnimationSystem
{
public:
	AnimationSystem(const std::string& filePath);

	void Update();

	void SetAnimation(const std::string& animName);
	void AddTransition(const std::string& from, const std::string& to, const std::function<bool()>& condition);

	AnimationTexture& GetTexture() const;

	std::string GetCurrentAnimation() const;

	std::vector<std::string> GetAnimationList() const;

	bool AnimationIsFinished() const;
private:
	std::string animState = "";
	std::unordered_map<std::string, Animation> animations;
	std::unordered_multimap<std::string, std::pair<std::string, std::function<bool()>>> transitions;
};


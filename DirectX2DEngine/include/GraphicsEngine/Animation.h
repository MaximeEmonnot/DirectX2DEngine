#pragma once
#include <vector>
#include <string>

#include "AnimationTexture.h"

class Animation {
public:
	enum class AnimationMode
	{
		NoLoop,
		LoopingAll,
		LoopingBack
	};
public:
	Animation(std::vector<std::string> frames, float holdTime, AnimationMode animMode);

	void				Update();

	AnimationTexture&	GetTexture() const;

	void				Reset();
	bool				IsFinished() const;
private:
	void				Advance();
private:
	std::vector<std::string>	frames;

	const std::string			spriteName;
	const AnimationMode			animMode;
	float						curFrameTime = 0.0f;
	float						holdTime;
	int							iCurFrame = 0;
	int							threshold = 1;
	bool						bIsFinished = false;
};

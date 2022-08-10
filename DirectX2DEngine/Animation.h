#pragma once
#include <vector>
#include <string>

#include "Texture.h"

class Animation {
public:
	enum AnimationMode
	{
		NoLoop,
		LoopingAll,
		LoopingBack
	};
public:
	Animation(std::vector<std::string> frames, float holdTime, AnimationMode animMode);

	void Update();

	Texture GetTexture() const;

	void Reset();
	bool IsFinished() const;
private:
	void Advance();
private:
	std::vector<std::string> frames;

	const std::string spriteName;
	const AnimationMode animMode;
	float curFrameTime = 0.0f;
	float holdTime;
	int iCurFrame = 0;
	int threshold = 1;
	bool bIsFinished = false;
};

#include "Animation.h"

#include "TextureFactory.h"
#include "Timer.h"

Animation::Animation(std::vector<std::string> frames, float holdTime, AnimationMode animMode)
	:
	frames(frames),
	animMode(animMode),
	holdTime(holdTime)
{
}

void Animation::Update()
{
	curFrameTime += DELTATIME;
	while (curFrameTime > holdTime) Advance();
}

AnimationTexture& Animation::GetTexture() const
{
	return ANIMATION_TEXTURE(frames.at(iCurFrame));
}

void Animation::Reset()
{
	iCurFrame = 0;
	threshold = 1;
	bIsFinished = false;
}

bool Animation::IsFinished() const
{
	return bIsFinished;
}

void Animation::Advance()
{
	iCurFrame += threshold;
	if (iCurFrame >= frames.size() || iCurFrame < 0) {
		switch (animMode)
		{
		case AnimationMode::NoLoop:
			iCurFrame = static_cast<int>(frames.size() - 1);
			bIsFinished = true;
			break;
		case AnimationMode::LoopingAll:
			iCurFrame = 0;
			break;
		case AnimationMode::LoopingBack:
			bIsFinished = (iCurFrame < 0);
			threshold *= -1;
			iCurFrame += 2 * threshold;
			break;
		default:
			break;
		}
	}
	else bIsFinished = false;
	curFrameTime -= holdTime;
}

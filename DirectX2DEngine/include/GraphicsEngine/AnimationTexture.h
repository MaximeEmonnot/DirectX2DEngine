#pragma once

#include "Texture.h"
#include "MathEngine/Vec2D.h"

class AnimationTexture : public Texture
{
public:
	AnimationTexture(const std::string& filePath);

	virtual ~AnimationTexture() override = default;

	Texture& operator()();

	IVec2D	GetCenter() const;

private:
	IVec2D	center;
};


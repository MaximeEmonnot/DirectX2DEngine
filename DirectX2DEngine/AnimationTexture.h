#pragma once
#include "Texture.h"
#include "Vec2D.h"

class AnimationTexture : public Texture
{
public:
	AnimationTexture(const std::string& filepath);

	virtual ~AnimationTexture() override = default;

	Texture& operator()();

	IVec2D GetCenter() const;

private:
	IVec2D center;
};


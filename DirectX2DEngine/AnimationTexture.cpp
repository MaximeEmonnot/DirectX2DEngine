#include "AnimationTexture.h"

AnimationTexture::AnimationTexture(const std::string& filepath)
	:
	Texture(filepath)
{
	const unsigned char* tga_data = LoadTGA(filepath);

	// Reading Center Data from the first pixel (This pixel won't be drawn because its alpha value is null)
	center = IVec2D(tga_data[0] << 4 | tga_data[1] >> 4,
					(tga_data[1] & 15) << 8 | tga_data[2]);
	center.x -= width / 2;
	center.y = -center.y + height / 2;
}

Texture& AnimationTexture::operator()()
{
	return *dynamic_cast<Texture*>(this);
}

IVec2D AnimationTexture::GetCenter() const
{
	return center;
}

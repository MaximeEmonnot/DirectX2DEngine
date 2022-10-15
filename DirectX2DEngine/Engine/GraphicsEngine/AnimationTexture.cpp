#include "GraphicsEngine/AnimationTexture.h"

AnimationTexture::AnimationTexture(const std::string& filePath)
	:
	Texture(filePath)
{
	const unsigned char* tga_data = LoadTGA(filePath);

	// Reading Center Data from the first pixel (This pixel won't be drawn because its alpha value is null)
	center = IVec2D(tga_data[0] << 4 | tga_data[1] >> 4,
					(tga_data[1] & 15) << 8 | tga_data[2]);
	center.x -= width / 2;
	center.y = -center.y + height / 2;

	delete[] tga_data;
}

Texture& AnimationTexture::operator()()
{
	return *dynamic_cast<Texture*>(this);
}

IVec2D AnimationTexture::GetCenter() const
{
	return center;
}

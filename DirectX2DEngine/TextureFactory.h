#pragma once
#include <map>
#include <string>
#include <memory>
#include "Texture.h"

#define TEXTURE(path, bForAnim) TextureFactory::GetInstance().GetTexture(path, bForAnim)

class TextureFactory
{
public:
	TextureFactory() = default;
	TextureFactory(const TextureFactory&) = delete;
	TextureFactory& operator=(const TextureFactory&) = delete;

	static TextureFactory& GetInstance();

	Texture& GetTexture(const std::string& path, bool bIsForAnimation = false);

private:
	static std::unique_ptr<TextureFactory> pInstance;

	std::map<std::string, std::shared_ptr<Texture>> textureMap;
};


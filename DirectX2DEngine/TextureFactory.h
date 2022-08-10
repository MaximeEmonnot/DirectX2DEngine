#pragma once
#include <map>
#include <string>
#include <memory>
#include "Texture.h"

#define TEXTURE(path) TextureFactory::GetInstance().GetTexture(path)

class TextureFactory
{
public:
	TextureFactory() = default;
	TextureFactory(const TextureFactory&) = delete;
	TextureFactory& operator=(const TextureFactory&) = delete;

	static TextureFactory& GetInstance();

	Texture& GetTexture(const std::string& path);

private:
	static std::unique_ptr<TextureFactory> pInstance;

	std::map<std::string, std::shared_ptr<Texture>> textureMap;
};


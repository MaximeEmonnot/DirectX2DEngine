#include "TextureFactory.h"

#include "AnimationTexture.h"

// STATIC VARIABLES INITIALIZATION
std::unique_ptr<TextureFactory> TextureFactory::pInstance = nullptr;

TextureFactory& TextureFactory::GetInstance()
{
    if (!pInstance)
        pInstance = std::make_unique<TextureFactory>();
    return *pInstance;
}

Texture& TextureFactory::GetTexture(const std::string& path, bool bIsForAnimation)
{
    if (!textureMap.contains(path)) {
        if (bIsForAnimation) textureMap.emplace(path, std::make_shared<AnimationTexture>(path));
        else textureMap.emplace(path, std::make_shared<Texture>(path));
    }
    return *textureMap.at(path);
}

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

Texture& TextureFactory::GetTexture(const std::string& path)
{
    if (!textureMap.contains(path)) 
        textureMap.emplace(path, std::make_shared<Texture>(path));
    return *textureMap.at(path);
}

AnimationTexture& TextureFactory::GetAnimationTexture(const std::string& path)
{
    if (!textureMap.contains(path))
        textureMap.emplace(path, std::make_shared<AnimationTexture>(path));
    const std::shared_ptr<AnimationTexture> out = std::dynamic_pointer_cast<AnimationTexture>(textureMap.at(path));
    return *out;
}

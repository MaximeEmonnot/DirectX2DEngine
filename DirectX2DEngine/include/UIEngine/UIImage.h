#pragma once

#include "AUIElement.h"

#include <memory>

#include "GraphicsEngine/Texture.h"
#include "GraphicsEngine/TextureModel.h"

class UIImage : public AUIElement
{
public:
	UIImage() = default;
	UIImage(const FRect& position, const Texture& texture);
	UIImage(const FRect& position, const std::string& texturePath);

	void SetPosition(const FRect& newPosition) override;
	void SetTexture(const Texture& newTexture) const;
	void SetInverted(bool bValue) const;

	void Render() const override;

private:
	std::shared_ptr<TextureModel> pModel;
};


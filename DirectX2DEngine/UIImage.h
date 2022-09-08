#pragma once
#include <memory>

#include "AUIElement.h"
#include "Texture.h"
#include "TextureModel.h"

class UIImage : public AUIElement
{
public:
	UIImage() = default;
	UIImage(const FRect& position, const Texture& texture);
	UIImage(const FRect& position, const std::string& texture_path);

	void SetPosition(const FRect& new_position) override;
	void SetTexture(const Texture& texture) const;
	void SetInverted(bool bValue) const;

	void Render() const override;

private:
	std::shared_ptr<TextureModel> model;
};


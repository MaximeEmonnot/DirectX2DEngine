#include "UIEngine/UIImage.h"

UIImage::UIImage(const FRect& position, const Texture& texture)
	:
	AUIElement(position),
	model(std::make_shared<TextureModel>(texture))
{
	model->Initialize();
	model->SetRectangle(position);
	model->SetDepth(depth);
}

UIImage::UIImage(const FRect& position, const std::string& texture_path)
	:
	AUIElement(position),
	model(std::make_shared<TextureModel>(texture_path))
{
	model->Initialize();
	model->SetRectangle(position);
	model->SetDepth(depth);
}

void UIImage::SetPosition(const FRect& new_position)
{
	AUIElement::SetPosition(new_position);
	model->SetRectangle(new_position);
}

void UIImage::SetTexture(const Texture& texture) const
{
	model->SetTexture(texture);
}

void UIImage::SetInverted(bool bValue) const
{
	model->SetInverted(bValue);
}

void UIImage::Render() const
{
	model->Render();
}

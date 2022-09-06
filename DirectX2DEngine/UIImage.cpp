#include "UIImage.h"

UIImage::UIImage(const FRect& position, const Texture& texture)
	:
	AUIElement(position),
	model(std::make_shared<TextureModel>(texture))
{
	model->Initialize();
	model->SetPosition(position.pos);
	model->SetDepth(depth);
}

UIImage::UIImage(const FRect& position, const std::string& texture_path)
	:
	AUIElement(position),
	model(std::make_shared<TextureModel>(texture_path))
{
	model->Initialize();
	model->SetPosition(position.pos);
}

void UIImage::SetPosition(const FRect& new_position)
{
	AUIElement::SetPosition(new_position);
	model->SetPosition(new_position.pos);
}

void UIImage::SetTexture(const Texture& texture) const
{
	model->SetTexture(texture);
}

void UIImage::Render() const
{
	model->Render();
}

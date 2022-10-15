#include "UIEngine/UIImage.h"

UIImage::UIImage(const FRect& position, const Texture& texture)
	:
	AUIElement(position),
	pModel(std::make_shared<TextureModel>(texture))
{
	pModel->Initialize();
	pModel->SetRectangle(position);
	pModel->SetDepth(depth);
}

UIImage::UIImage(const FRect& position, const std::string& texturePath)
	:
	AUIElement(position),
	pModel(std::make_shared<TextureModel>(texturePath))
{
	pModel->Initialize();
	pModel->SetRectangle(position);
	pModel->SetDepth(depth);
}

void UIImage::SetPosition(const FRect& newPosition)
{
	AUIElement::SetPosition(newPosition);
	pModel->SetRectangle(newPosition);
}

void UIImage::SetTexture(const Texture& newTexture) const
{
	pModel->SetTexture(newTexture);
}

void UIImage::SetInverted(bool bValue) const
{
	pModel->SetInverted(bValue);
}

void UIImage::Render() const
{
	pModel->Render();
}

#include "UIEngine/UIProgressBar.h"

UIProgressBar::UIProgressBar(const FRect& position, const DirectX::XMFLOAT4& color, ProgressDirection direction)
	:
	AUIElement(position),
	pBackgroundModel(std::make_shared<ColorModel>()),
	pFillModel(std::make_shared<ColorModel>()),
	direction(direction)
{
	pBackgroundModel->Initialize();
	pBackgroundModel->SetColor(DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f));
	pBackgroundModel->SetRectangle(position);
	pBackgroundModel->SetDepth(depth);

	pFillModel->Initialize();
	pFillModel->SetColor(color);
	pFillModel->SetRectangle(position);
	pFillModel->SetDepth(depth);
}

void UIProgressBar::SetPercentage(float newPercentage)
{
	percentage = newPercentage;
	FRect new_rectangle = position;
	const float left = position.pos.x - position.width / 2;
	const float right = position.pos.x + position.width / 2;
	const float bottom = position.pos.y - position.height / 2;
	const float top = position.pos.y + position.height / 2;
	switch(direction)
	{
	case ProgressDirection::LeftToRight:
		new_rectangle.width *= percentage;
		new_rectangle.pos.x = right - (right - position.pos.x) * percentage;
		break;
	case ProgressDirection::RightToLeft:
		new_rectangle.width *= percentage;
		new_rectangle.pos.x = left + (position.pos.x - left) * percentage;
		break;
	case ProgressDirection::TopToBottom:
		new_rectangle.height *= percentage;
		new_rectangle.pos.y = bottom + (position.pos.y - bottom) * percentage;
		break;
	case ProgressDirection::BottomToTop:
		new_rectangle.height *= percentage;
		new_rectangle.pos.y = top - (top - position.pos.y) * percentage;
		break;
	case ProgressDirection::BothHorizontal:
		new_rectangle.width *= percentage;
		break;
	case ProgressDirection::BothVertical:
		new_rectangle.height *= percentage;
		break;
	case ProgressDirection::AllSides:
		new_rectangle *= percentage;
		break;
	default: ;
	}
	pFillModel->SetRectangle(new_rectangle);
}

void UIProgressBar::Render() const
{
	pFillModel->Render();
	pBackgroundModel->Render();
}

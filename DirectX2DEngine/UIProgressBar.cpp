#include "UIProgressBar.h"

UIProgressBar::UIProgressBar(const FRect& position, const DirectX::XMFLOAT4& color, ProgressDirection direction)
	:
	AUIElement(position),
	model(std::make_shared<ColorModel>()),
	direction(direction)
{
	model->Initialize();
	model->SetColor(color);
	model->SetRectangle(position);
	model->SetVisibility(true);
	model->SetDepth(depth);
}

void UIProgressBar::SetPercentage(float new_percentage)
{
	percentage = new_percentage;
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
	model->SetRectangle(new_rectangle);
}

void UIProgressBar::Render() const
{
	model->Render();
}

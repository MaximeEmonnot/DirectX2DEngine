#include "UIEngine/UIButton.h"

#include "CoreEngine/Mouse.h"

UIButton::UIButton(const FRect& position, const DirectX::XMFLOAT4& base_color, const DirectX::XMFLOAT4& hover_color, const DirectX::XMFLOAT4& click_color)
	:
	AUIElement(position),
	model(std::make_shared<ColorModel>(position, base_color)),
	baseColor(base_color),
	hoverColor(hover_color),
	clickColor(click_color),
	task([&]{})
{
	model->Initialize();
	model->SetDepth(depth);
}

void UIButton::SetTask(std::function<void()> new_task)
{
	task = new_task;
}

void UIButton::Update()
{
	if (position.ContainsVec2D(MOUSE_POSITION)){
		if (MOUSE.Read() == Mouse::EventType::LPress) {
			model->SetColor(clickColor);
			task();
		}
		else model->SetColor(hoverColor);
	}
	else model->SetColor(baseColor);
}

void UIButton::Render() const
{
	model->Render();
}

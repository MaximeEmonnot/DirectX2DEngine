#include "UIEngine/UIButton.h"

#include "CoreEngine/Mouse.h"

UIButton::UIButton(const FRect& position, const DirectX::XMFLOAT4& baseColor, const DirectX::XMFLOAT4& hoverColor, const DirectX::XMFLOAT4& clickColor)
	:
	AUIElement(position),
	pModel(std::make_shared<ColorModel>(position, baseColor)),
	baseColor(baseColor),
	hoverColor(hoverColor),
	clickColor(clickColor),
	task([&]{})
{
	pModel->Initialize();
	pModel->SetDepth(depth);
}

void UIButton::SetTask(std::function<void()> newTask)
{
	task = newTask;
}

void UIButton::Update()
{
	if (position.ContainsVec2D(MOUSE_POSITION)){
		if (MOUSE.Read() == Mouse::EventType::LPress) {
			pModel->SetColor(clickColor);
			task();
		}
		else pModel->SetColor(hoverColor);
	}
	else pModel->SetColor(baseColor);
}

void UIButton::Render() const
{
	pModel->Render();
}

#include "UICanvas_Combat.h"

UICanvas_Combat::UICanvas_Combat()
	:
	button(CreateUIElement<UIButton>(FRect(15.f, 25.f, 150.f, 25.f), DirectX::XMFLOAT4(1.f, 0.f, 1.f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 0.f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
	text(CreateUIElement<UIText>(FRect(-50.f, -100.f, 450.f, 25.f), L"Test ahaha", L"Cambria", 42.f, D2D1::ColorF(D2D1::ColorF::Blue)))
{
}

void UICanvas_Combat::Update()
{
	button->Update();
}

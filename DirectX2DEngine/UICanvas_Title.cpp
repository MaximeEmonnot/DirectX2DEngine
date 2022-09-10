#include "UICanvas_Title.h"

#include "Engine.h"

UICanvas_Title::UICanvas_Title()
	:
	titleLogo(CreateUIElement<UIImage>(FRect(0.f, 50.f, 538.f, 412.f), "Textures/logo.tga")),
	startButton(CreateUIElement<UIButton>(FRect(0.f, -200.f, 150.f, 75.f), DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
	startButtonTitle(CreateUIElement<UIText>(FRect(0.f, -200.f, 150.f, 75.f), L"Start Game", L"Cambria", 25.f, D2D1::ColorF(D2D1::ColorF::Black)))
{
	startButton->SetTask([&]
		{
			ENGINE.SetLevel(1);
		});
}

void UICanvas_Title::Update()
{
	startButton->Update();
}

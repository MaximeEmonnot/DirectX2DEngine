#include "UICanvas_Title.h"

#include "ConnectionLevel.h"
#include "Engine.h"
#include "NetworkSystem.h"
#include "SinglePlayerSelectionLevel.h"


UICanvas_Title::UICanvas_Title()
	:
	titleLogo(CreateUIElement<UIImage>(FRect(0.f, 50.f, 538.f, 412.f), "Textures/logo.tga")),
	singlePlayerButton(CreateUIElement<UIButton>(FRect(-150.f, -200.f, 150.f, 75.f), DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
	singlePlayerButtonTitle(CreateUIElement<UIText>(FRect(-150.f, -200.f, 150.f, 75.f), L"Single Player", L"Cambria", 25.f, D2D1::ColorF(D2D1::ColorF::Black))),
	multiPlayerButton(CreateUIElement<UIButton>(FRect(150.f, -200.f, 150.f, 75.f), DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
	multiPlayerButtonTitle(CreateUIElement<UIText>(FRect(150.f, -200.f, 150.f, 75.f), L"Multi Player", L"Cambria", 25.f, D2D1::ColorF(D2D1::ColorF::Black)))

{
	singlePlayerButton->SetTask([&]
		{
			ENGINE.SetLevel<SinglePlayerSelectionLevel>();
		});

	multiPlayerButton->SetTask([&]
		{
			ENGINE.SetLevel<ConnectionLevel>();
		});
}

void UICanvas_Title::Update()
{
	singlePlayerButton->Update();
	multiPlayerButton->Update();
}

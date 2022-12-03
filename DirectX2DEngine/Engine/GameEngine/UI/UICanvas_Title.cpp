#include "GameEngine/UI/UICanvas_Title.h"

#include "NetworkEngine/NetworkSystem.h"
#include "MainEngine/Engine.h"
#include "GameEngine/Levels/ConnectionLevel.h"
#include "GameEngine/Levels/SinglePlayerSelectionLevel.h"


UICanvas_Title::UICanvas_Title()
	:
	pTitleLogo(CreateUIElement<UIImage>(FRect(0.f, 50.f, 538.f, 412.f), "Textures/logo.tga")),
	pSinglePlayerButton(CreateUIElement<UIButton>(FRect(-150.f, -200.f, 150.f, 75.f), DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
	pSinglePlayerButtonTitle(CreateUIElement<UIText>(FRect(-150.f, -200.f, 150.f, 75.f), L"Single Player", L"Cambria", 25.f, D2D1::ColorF(D2D1::ColorF::Black))),
	pMultiPlayerButton(CreateUIElement<UIButton>(FRect(150.f, -200.f, 150.f, 75.f), DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
	pMultiPlayerButtonTitle(CreateUIElement<UIText>(FRect(150.f, -200.f, 150.f, 75.f), L"Multi Player", L"Cambria", 25.f, D2D1::ColorF(D2D1::ColorF::Black)))

{
	pSinglePlayerButton->SetTask([&]
		{
			ENGINE.SetLevel<SinglePlayerSelectionLevel>();
			ENGINE.GetCurrentLevel()->BeginLevel();
		});

	pMultiPlayerButton->SetTask([&]
		{
			ENGINE.SetLevel<ConnectionLevel>();
			ENGINE.GetCurrentLevel()->BeginLevel();
		});
}

void UICanvas_Title::Update()
{
	pSinglePlayerButton->Update();
	pMultiPlayerButton->Update();
}

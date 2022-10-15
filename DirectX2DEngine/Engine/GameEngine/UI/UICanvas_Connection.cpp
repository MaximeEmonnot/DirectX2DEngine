#include "GameEngine/UI/UICanvas_Connection.h"

#include "NetworkEngine/NetworkSystem.h"
#include "MainEngine/Engine.h"
#include "GameEngine/Levels/MultiPlayerSelectionLevel.h"

UICanvas_Connection::UICanvas_Connection()
	:
	pIpAddressTextBox(CreateUIElement<UITextBox>(FRect(0.f, 150.f, 600.f, 75.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f), L"Cambria", L"Server IP Address")),
	pPortTextBox(CreateUIElement<UITextBox>(FRect(0.f, 0.f, 600.f, 75.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f), L"Cambria", L"Server port")),
	pConnectionButton(CreateUIElement<UIButton>(FRect(0.f, -150.f, 150.f, 75.f), DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
	pConnectionButtonTitle(CreateUIElement<UIText>(FRect(0.f, -150.f, 150.f, 75.f), L"Connect to Server", L"Cambria", 16.f, D2D1::ColorF(D2D1::ColorF::Black)))
{
	pConnectionButton->SetTask([&]
	{
		NETWORK.ConnectTo(pIpAddressTextBox->GetText(), std::stoi(pPortTextBox->GetText()));
		ENGINE.SetLevel<MultiPlayerSelectionLevel>();
	});
}

void UICanvas_Connection::Update()
{
	pIpAddressTextBox->Update();
	pPortTextBox->Update();
	pConnectionButton->Update();
}

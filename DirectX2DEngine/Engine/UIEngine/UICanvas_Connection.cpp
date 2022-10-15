#include "UIEngine/UICanvas_Connection.h"

#include "MainEngine/Engine.h"
#include "GameEngine/MultiPlayerSelectionLevel.h"
#include "NetworkEngine/NetworkSystem.h"

UICanvas_Connection::UICanvas_Connection()
	:
	ipAddressTextBox(CreateUIElement<UITextBox>(FRect(0.f, 150.f, 600.f, 75.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f), L"Cambria", L"Server IP Address")),
	portTextBox(CreateUIElement<UITextBox>(FRect(0.f, 0.f, 600.f, 75.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f), L"Cambria", L"Server port")),
	connectionButton(CreateUIElement<UIButton>(FRect(0.f, -150.f, 150.f, 75.f), DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
	connectionButtonTitle(CreateUIElement<UIText>(FRect(0.f, -150.f, 150.f, 75.f), L"Connect to Server", L"Cambria", 16.f, D2D1::ColorF(D2D1::ColorF::Black)))
{
	connectionButton->SetTask([&]
	{
		NETWORK.ConnectTo(ipAddressTextBox->GetText(), std::stoi(portTextBox->GetText()));
		ENGINE.SetLevel<MultiPlayerSelectionLevel>();
	});
}

void UICanvas_Connection::Update()
{
	ipAddressTextBox->Update();
	portTextBox->Update();
	connectionButton->Update();
}

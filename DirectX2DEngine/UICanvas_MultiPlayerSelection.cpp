#include "UICanvas_MultiPlayerSelection.h"

#include "Engine.h"
#include "MultiPlayerCombatLevel.h"
#include "MultiPlayerSelectionLevel.h"
#include "NetworkSystem.h"
#include "Timer.h"

UICanvas_MultiPlayerSelection::UICanvas_MultiPlayerSelection()
	:
	solBadguyImage(CreateUIElement<UIImage>(FRect(-100.f, 78.75f, 149.86f, 317.5f), "Textures/SolBadguy/selection.tga")),
	solBadguyButton(CreateUIElement<UIButton>(FRect(-100.f, -200.f, 150.f, 75.f), DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
	solBadguyButtonTitle(CreateUIElement<UIText>(FRect(-100.f, -200.f, 150.f, 75.f), L"Sol Badguy", L"Cambria", 25.f, D2D1::ColorF(D2D1::ColorF::Black))),
	roboKyImage(CreateUIElement<UIImage>(FRect(100.f, 78.75f, 256.413f, 317.5f), "Textures/RoboKy/selection.tga")),
	roboKyButton(CreateUIElement<UIButton>(FRect(100.f, -200.f, 150.f, 75.f), DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
	roboKyButtonTitle(CreateUIElement<UIText>(FRect(100.f, -200.f, 150.f, 75.f), L"Robo Ky", L"Cambria", 25.f, D2D1::ColorF(D2D1::ColorF::Black)))
{
	solBadguyButton->SetTask(
		[&]
		{
			if (std::shared_ptr<MultiPlayerSelectionLevel> level = std::dynamic_pointer_cast<MultiPlayerSelectionLevel>(ENGINE.GetCurrentLevel()))
			{
				std::vector<uint8_t> input;

				switch(NETWORK.GetPlace())
				{
				case 1: // If the client is the first one connected, we should wait for another client to send data
				{
					input = NETWORK.ReceiveData();
					std::vector<uint8_t> output;
					output.emplace_back(1);
					NETWORK.SendData(output);
				}
					break;
				case 2: // If the client is the second one connected, we should send the data first
				{
					std::vector<uint8_t> output;
					output.emplace_back(1);
					NETWORK.SendData(output);
					input = NETWORK.ReceiveData();
				}
					break;
				default:
					break;
				}

				ENGINE.SetLevel<MultiPlayerCombatLevel>();
				if (std::shared_ptr<MultiPlayerCombatLevel> new_level = std::dynamic_pointer_cast<MultiPlayerCombatLevel>(ENGINE.GetCurrentLevel()))
				{
					new_level->SetSelection(std::pair(1, static_cast<int>(input.at(0))));
					new_level->BeginLevel();
				}
			}
		}
		);

	roboKyButton->SetTask(
		[&]
		{
			if (std::shared_ptr<MultiPlayerSelectionLevel> level = std::dynamic_pointer_cast<MultiPlayerSelectionLevel>(ENGINE.GetCurrentLevel()))
			{
				std::vector<uint8_t> input;

				switch (NETWORK.GetPlace())
				{
				case 1: // If the client is the first one connected, we should wait for another client to send data
				{
					input = NETWORK.ReceiveData();
					TICKCLOCK // We reset the DeltaTime value as we use a blocking call on the main thread
					std::vector<uint8_t> output;
					output.emplace_back(2);
					NETWORK.SendData(output);
				}
				break;
				case 2: // If the client is the second one connected, we should send the data first
				{
					std::vector<uint8_t> output;
					output.emplace_back(2);
					NETWORK.SendData(output);
					input = NETWORK.ReceiveData();
					TICKCLOCK // We reset the DeltaTime value as we use a blocking call on the main thread
				}
				break;
				default:
					break;
				}

				ENGINE.SetLevel<MultiPlayerCombatLevel>();
				if (std::shared_ptr<MultiPlayerCombatLevel> new_level = std::dynamic_pointer_cast<MultiPlayerCombatLevel>(ENGINE.GetCurrentLevel()))
				{
					new_level->SetSelection(std::pair(2, static_cast<int>(input.at(0))));
					new_level->BeginLevel();
				}
			}
		}
		);
}

void UICanvas_MultiPlayerSelection::Update()
{
	solBadguyButton->Update();
	roboKyButton->Update();
}

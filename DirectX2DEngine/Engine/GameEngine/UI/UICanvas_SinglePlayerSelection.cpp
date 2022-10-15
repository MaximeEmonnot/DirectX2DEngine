#include "GameEngine/UI/UICanvas_SinglePlayerSelection.h"

#include "MainEngine/Engine.h"
#include "GameEngine/Levels/SinglePlayerCombatLevel.h"
#include "GameEngine/Levels/SinglePlayerSelectionLevel.h"

UICanvas_SinglePlayerSelection::UICanvas_SinglePlayerSelection()
	:
	pSolBadguyImage(CreateUIElement<UIImage>(FRect(-100.f, 78.75f, 149.86f, 317.5f), "Textures/SolBadguy/selection.tga")),
	pSolBadguyButton(CreateUIElement<UIButton>(FRect(-100.f, -200.f, 150.f, 75.f), DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
	pSolBadguyButtonTitle(CreateUIElement<UIText>(FRect(-100.f, -200.f, 150.f, 75.f), L"Sol Badguy", L"Cambria", 25.f, D2D1::ColorF(D2D1::ColorF::Black))),
	pRoboKyImage(CreateUIElement<UIImage>(FRect(100.f, 78.75f, 256.413f, 317.5f), "Textures/RoboKy/selection.tga")),
	pRoboKyButton(CreateUIElement<UIButton>(FRect(100.f, -200.f, 150.f, 75.f), DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
	pRoboKyButtonTitle(CreateUIElement<UIText>(FRect(100.f, -200.f, 150.f, 75.f), L"Robo Ky", L"Cambria", 25.f, D2D1::ColorF(D2D1::ColorF::Black)))
{
	pSolBadguyButton->SetTask(
		[&]
		{
			if(std::shared_ptr<SinglePlayerSelectionLevel> level = std::dynamic_pointer_cast<SinglePlayerSelectionLevel>(ENGINE.GetCurrentLevel()))
			{
				if (level->GetSelection().first == 0) level->SetSelectionValue(true, 1);
				else if (level->GetSelection().second == 0) {
					level->SetSelectionValue(false, 1);
					const std::pair<int, int> selection = level->GetSelection();
					ENGINE.SetLevel<SinglePlayerCombatLevel>();
					if (std::shared_ptr<SinglePlayerCombatLevel> new_level = std::dynamic_pointer_cast<SinglePlayerCombatLevel>(ENGINE.GetCurrentLevel()))
					{
						new_level->SetSelection(selection);
						new_level->BeginLevel();
					}
				}
			}
		}
	);

	pRoboKyButton->SetTask(
		[&]
		{
			if (std::shared_ptr<SinglePlayerSelectionLevel> level = std::dynamic_pointer_cast<SinglePlayerSelectionLevel>(ENGINE.GetCurrentLevel()))
			{
				if (level->GetSelection().first == 0) level->SetSelectionValue(true, 2);
				else if (level->GetSelection().second == 0) {
					level->SetSelectionValue(false, 2);
					const std::pair<int, int> selection = level->GetSelection();
					ENGINE.SetLevel<SinglePlayerCombatLevel>();
					if (std::shared_ptr<SinglePlayerCombatLevel> new_level = std::dynamic_pointer_cast<SinglePlayerCombatLevel>(ENGINE.GetCurrentLevel()))
					{
						new_level->SetSelection(selection);
						new_level->BeginLevel();
					}
				}
			}
		}
	);
}

void UICanvas_SinglePlayerSelection::Update()
{
	pSolBadguyButton->Update();
	pRoboKyButton->Update();
}

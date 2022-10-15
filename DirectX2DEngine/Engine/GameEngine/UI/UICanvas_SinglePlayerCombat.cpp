#include "GameEngine/UI/UICanvas_SinglePlayerCombat.h"

UICanvas_SinglePlayerCombat::UICanvas_SinglePlayerCombat(std::shared_ptr<BaseFighter> player, std::shared_ptr<BaseFighter> enemy)
	:
	pPlayerHealthBar(CreateUIElement<UIProgressBar>(FRect(-240.f, 250.f, 300.f, 25.f), DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f), UIProgressBar::ProgressDirection::LeftToRight)),
	pPlayerIcon(CreateUIElement<UIImage>(FRect(-50.f, 250.f, 80.f, 100.f), player->GetIcon())),
	pPlayerName(CreateUIElement<UIText>(FRect(-250.f, 275.f, 200.f, 25.f), player->GetName(), L"Cambria", 22.f, D2D1::ColorF(D2D1::ColorF::Red))),
	pEnemyHealthBar(CreateUIElement<UIProgressBar>(FRect(240.f, 250.f, 300.f, 25.f), DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f), UIProgressBar::ProgressDirection::RightToLeft)),
	pEnemyIcon(CreateUIElement<UIImage>(FRect(50.f, 250.f, 80.f, 100.f), enemy->GetIcon())),
	pEnemyName(CreateUIElement<UIText>(FRect(250.f, 275.f, 200.f, 25.f), enemy->GetName(), L"Cambria", 22.f, D2D1::ColorF(D2D1::ColorF::Red)))
{
	pEnemyIcon->SetInverted(true);
}

void UICanvas_SinglePlayerCombat::Update()
{
	// The following percentage is just an example
	pPlayerHealthBar->SetPercentage(0.85f);
	pEnemyHealthBar->SetPercentage(0.15f);
}

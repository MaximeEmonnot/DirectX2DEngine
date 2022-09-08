#include "UICanvas_Combat.h"

#include "SoundSystem.h"

UICanvas_Combat::UICanvas_Combat()
	:
	playerHealthBar(CreateUIElement<UIProgressBar>(FRect(-240.f, 250.f, 300.f, 25.f), DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f), UIProgressBar::ProgressDirection::LeftToRight)),
	playerIcon(CreateUIElement<UIImage>(FRect(-50.f, 250.f, 80.f, 100.f), "Textures/SolBadguy/icon.tga")),
	playerName(CreateUIElement<UIText>(FRect(-250.f, 275.f, 100.f, 25.f), L"Sol Badguy", L"Cambria", 16.f, D2D1::ColorF(D2D1::ColorF::Red))),
	enemyHealthBar(CreateUIElement<UIProgressBar>(FRect(240.f, 250.f, 300.f, 25.f), DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f), UIProgressBar::ProgressDirection::RightToLeft)),
	enemyIcon(CreateUIElement<UIImage>(FRect(50.f, 250.f, 80.f, 100.f), "Textures/RoboKy/icon.tga")),
	enemyName(CreateUIElement<UIText>(FRect(250.f, 275.f, 100.f, 25.f), L"Robo Ky", L"Cambria", 16.f, D2D1::ColorF(D2D1::ColorF::Red)))
{
	enemyIcon->SetInverted(true);
}

void UICanvas_Combat::Update()
{
	playerHealthBar->SetPercentage(0.25f);
	enemyHealthBar->SetPercentage(0.86f);
}

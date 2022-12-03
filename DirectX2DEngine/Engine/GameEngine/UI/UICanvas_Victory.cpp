#include "GameEngine/UI/UICanvas_Victory.h"

#include "CoreEngine/Keyboard.h"
#include "GameEngine/Levels/TitleLevel.h"
#include "MainEngine/Engine.h"

UICanvas_Victory::UICanvas_Victory(const std::string& winnerName, int playerNumber)
	:
	pVictoryText(CreateUIElement<UIText>(FRect( (playerNumber == 1 ? -1 : 1) * 200.f, -250.f, 250.f, 75.f), L"YOU WON", L"Cambria", 35.f, D2D1::ColorF(D2D1::ColorF::Red))),
	pVictoryBackground(CreateUIElement<UIImage>(FRect(0.f, 0.f, 640.f, 480.f), "Textures/" + winnerName + "/victory.tga"))
{
}

void UICanvas_Victory::Update()
{
	if (KBD.KeyIsPressed(VK_RETURN)) ENGINE.SetLevel<TitleLevel>();
}

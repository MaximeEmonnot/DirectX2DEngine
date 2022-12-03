#include "GameEngine/Levels/VictoryLevel.h"

#include <codecvt>
#include <locale>

#include "GameEngine/UI/UICanvas_Victory.h"

void VictoryLevel::Update()
{
	Level::Update();
}

void VictoryLevel::BeginLevel()
{
	Clear();

	CreateCanvas<UICanvas_Victory>(winnerFighter, playerNumber);
}

void VictoryLevel::SetWinner(const BaseFighter& newWinnerFighter, int newPlayerNumber)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	winnerFighter = converter.to_bytes(newWinnerFighter.GetName());
	std::erase_if(winnerFighter, [](char x) { return x == ' '; });
	playerNumber = newPlayerNumber;
}

#include "GameEngine/Levels/SinglePlayerSelectionLevel.h"

#include "GameEngine/UI/UICanvas_SinglePlayerSelection.h"

SinglePlayerSelectionLevel::SinglePlayerSelectionLevel()
{
	CreateCanvas<UICanvas_SinglePlayerSelection>();
}

void SinglePlayerSelectionLevel::Update()
{
	Level::Update();
}

void SinglePlayerSelectionLevel::BeginLevel()
{
	selection = std::pair(0, 0);
}

void SinglePlayerSelectionLevel::SetSelectionValue(bool bIsLeft, int value)
{
	if (bIsLeft) selection.first = value;
	else selection.second = value;
}

std::pair<int, int> SinglePlayerSelectionLevel::GetSelection() const
{
	return selection;
}

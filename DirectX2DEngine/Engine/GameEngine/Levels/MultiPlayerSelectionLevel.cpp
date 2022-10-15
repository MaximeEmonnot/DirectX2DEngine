#include "GameEngine/Levels/MultiPlayerSelectionLevel.h"

#include "GameEngine/UI/UICanvas_MultiPlayerSelection.h"

MultiPlayerSelectionLevel::MultiPlayerSelectionLevel()
{
	CreateCanvas<UICanvas_MultiPlayerSelection>();
}

void MultiPlayerSelectionLevel::Update()
{
	Level::Update();
}

void MultiPlayerSelectionLevel::BeginLevel()
{
}

void MultiPlayerSelectionLevel::SetSelectionValue(int value)
{
	selection = value;
}

int MultiPlayerSelectionLevel::GetSelection() const
{
	return selection;
}
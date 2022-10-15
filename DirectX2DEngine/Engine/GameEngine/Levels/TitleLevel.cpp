#include "GameEngine/Levels/TitleLevel.h"

#include "GameEngine/UI/UICanvas_Title.h"

TitleLevel::TitleLevel()
{
	CreateCanvas<UICanvas_Title>();
}

void TitleLevel::Update()
{
	Level::Update();
}

void TitleLevel::BeginLevel()
{}

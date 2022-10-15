#include "GameEngine/TitleLevel.h"

#include "UIEngine/UICanvas_Title.h"

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

#include "TitleLevel.h"

#include "UICanvas_Title.h"

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

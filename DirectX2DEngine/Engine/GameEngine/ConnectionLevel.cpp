#include "GameEngine/ConnectionLevel.h"

#include "UIEngine/UICanvas_Connection.h"

ConnectionLevel::ConnectionLevel()
{
	CreateCanvas<UICanvas_Connection>();
}

void ConnectionLevel::Update()
{
	Level::Update();
}

void ConnectionLevel::BeginLevel()
{
}

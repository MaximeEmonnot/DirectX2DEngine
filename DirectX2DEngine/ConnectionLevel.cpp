#include "ConnectionLevel.h"

#include "UICanvas_Connection.h"

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

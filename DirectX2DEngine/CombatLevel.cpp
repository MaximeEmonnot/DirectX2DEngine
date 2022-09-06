#include "CombatLevel.h"

#include "Background.h"
#include "Engine.h"
#include "LoggerManager.h"
#include "PlayerCharacter.h"
#include "Rect.h"
#include "SandBag.h"
#include "Timer.h"
#include "UICanvas_Test.h"

CombatLevel::CombatLevel()
{
	GetWorld().SpawnActor<SandBag>(FRect(50, 75, 32, 128), "Sandbag");
	GetWorld().SpawnActor<Background>(FVec2D(0, 300), "Background");
	GetWorld().SpawnActor<PlayerCharacter>(FVec2D(250, -125), "Player");

	CreateCanvas<UICanvas_Test>();
}

void CombatLevel::Update()
{
	Level::Update();
}

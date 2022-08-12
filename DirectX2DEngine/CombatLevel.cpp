#include "CombatLevel.h"

#include "Background.h"
#include "Engine.h"
#include "PlayerCharacter.h"
#include "Rect.h"
#include "SandBag.h"

CombatLevel::CombatLevel()
{
	GetWorld().SpawnActor<PlayerCharacter>(FVec2D(0, 0), "Player");
	GetWorld().SpawnActor<Background>(FVec2D(0, 200), "Background CHINA");
	GetWorld().SpawnActor<SandBag>(FRect(50, 75, 32, 128), "Sandbag");
}

void CombatLevel::Update()
{
	Level::Update();
}

void CombatLevel::Render()
{
	Level::Render();
}

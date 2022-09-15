#include "CombatLevel.h"

#include "Background.h"
#include "DummyController.h"
#include "Engine.h"
#include "LoggerManager.h"
#include "FighterCharacter.h"
#include "PlayerController.h"
#include "Rect.h"
#include "RoboKy.h"
#include "SandBag.h"
#include "SolBadguy.h"
#include "SoundSystem.h"
#include "Timer.h"
#include "UICanvas_Combat.h"

CombatLevel::CombatLevel()
{
	// Spawn Actors
	GetWorld().SpawnActor<Background>(FVec2D(0, 180), "Background");
	std::shared_ptr<FighterCharacter<PlayerController, SolBadguy>> player = GetWorld().SpawnActor<FighterCharacter<PlayerController, SolBadguy>>(FVec2D(-250, -125), "Player", 15);
	std::shared_ptr<FighterCharacter<DummyController, RoboKy>> enemy = GetWorld().SpawnActor<FighterCharacter<DummyController, RoboKy>>(FVec2D(250, -125), "Enemy", 12);

	player->GetFighter()->SetEnemy(enemy->GetFighter());
	enemy->GetFighter()->SetEnemy(player->GetFighter());

	// Create UI Canvas
	CreateCanvas<UICanvas_Combat>(player->GetFighter(), enemy->GetFighter());

	// Play background sounds
	SFX.Play("Sounds/BeJustOrBeDead.wav");
}

void CombatLevel::Update()
{
	Level::Update();
}

void CombatLevel::BeginLevel()
{
	
}


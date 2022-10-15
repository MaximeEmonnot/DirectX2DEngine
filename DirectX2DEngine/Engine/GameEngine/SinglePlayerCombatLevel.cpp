#include "GameEngine/SinglePlayerCombatLevel.h"

#include "GameEngine/Background.h"
#include "GameEngine/DummyController.h"
#include "MainEngine/Engine.h"
#include "GameEngine/FighterCharacter.h"
#include "GameEngine/PlayerController.h"
#include "SoundEngine/SoundSystem.h"
#include "CoreEngine/Timer.h"
#include "UIEngine/UICanvas_SinglePlayerCombat.h"

SinglePlayerCombatLevel::SinglePlayerCombatLevel()
{
	GetWorld().SpawnActor<Background>(FVec2D(0, 180), "Background");
}

void SinglePlayerCombatLevel::Update()
{
	Level::Update();
}

void SinglePlayerCombatLevel::BeginLevel()
{
	// Spawn Actors
	std::shared_ptr<FighterCharacter<PlayerController>> player = GetWorld().SpawnActor<FighterCharacter<PlayerController>>(FVec2D(-250, -125), "Player", static_cast<EFighterName>(selection.first - 1), 15);
	std::shared_ptr<FighterCharacter<DummyController>> enemy = GetWorld().SpawnActor<FighterCharacter<DummyController>>(FVec2D(250, -125), "Enemy", static_cast<EFighterName>(selection.second - 1), 12);

	player->GetFighter()->SetEnemy(enemy->GetFighter());
	enemy->GetFighter()->SetEnemy(player->GetFighter());

	// Create UI Canvas
	CreateCanvas<UICanvas_SinglePlayerCombat>(player->GetFighter(), enemy->GetFighter());

	// Play background sounds
	SFX.Play("Sounds/BeJustOrBeDead.wav");
}

void SinglePlayerCombatLevel::SetSelection(std::pair<int, int> _selection)
{
	selection = _selection;
}


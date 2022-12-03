#include "GameEngine/Levels/SinglePlayerCombatLevel.h"

#include "CoreEngine/Timer.h"
#include "SoundEngine/SoundSystem.h"
#include "MainEngine/Engine.h"
#include "GameEngine/Actors/Background.h"
#include "GameEngine/Levels/VictoryLevel.h"
#include "GameEngine/UI/UICanvas_SinglePlayerCombat.h"

void SinglePlayerCombatLevel::Update()
{
	Level::Update();

	if (!(pPlayer->GetFighter()->IsAlive() && pEnemy->GetFighter()->IsAlive()))
	{
		SFX.Pause("Sounds/BeJustOrBeDead.wav");
		ENGINE.SetLevel<VictoryLevel>();
		if (std::shared_ptr<VictoryLevel> new_level = std::dynamic_pointer_cast<VictoryLevel>(ENGINE.GetCurrentLevel()))
		{
			new_level->SetWinner(*(pPlayer->GetFighter()->IsAlive() ? pPlayer->GetFighter() : pEnemy->GetFighter()),
						         pPlayer->GetFighter()->IsAlive() ? 1 : 2);
			new_level->BeginLevel();
		}
	}
}

void SinglePlayerCombatLevel::BeginLevel()
{
	// Clear level from previous access
	Clear();

	// Spawn Actors
	GetWorld().SpawnActor<Background>(FVec2D(0, 180), "Background");
	pPlayer = GetWorld().SpawnActor<FighterCharacter<PlayerController>>(FVec2D(-250, -25), "Player", static_cast<EFighterName>(selection.first - 1), 15);
	pEnemy = GetWorld().SpawnActor<FighterCharacter<DummyController>>(FVec2D(250, -25), "Enemy", static_cast<EFighterName>(selection.second - 1), 12);

	// Linking Player and Enemy so they face each other
	pPlayer->GetFighter()->SetEnemy(pEnemy->GetFighter());
	pEnemy->GetFighter()->SetEnemy(pPlayer->GetFighter());

	// Create UI Canvas
	CreateCanvas<UICanvas_SinglePlayerCombat>(pPlayer->GetFighter(), pEnemy->GetFighter());

	// Play background sounds
	SFX.Play("Sounds/BeJustOrBeDead.wav");
}

void SinglePlayerCombatLevel::SetSelection(std::pair<int, int> _selection)
{
	selection = _selection;
}


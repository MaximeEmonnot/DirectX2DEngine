#include "MultiPlayerCombatLevel.h"

#include "Background.h"
#include "DummyController.h"
#include "FighterCharacter.h"
#include "NetworkSystem.h"
#include "PlayerController.h"
#include "SoundSystem.h"
#include "UICanvas_SinglePlayerCombat.h"

MultiPlayerCombatLevel::MultiPlayerCombatLevel()
{
	GetWorld().SpawnActor<Background>(FVec2D(0, 180), "Background");
}

void MultiPlayerCombatLevel::Update()
{
	Level::Update();
}

void MultiPlayerCombatLevel::BeginLevel()
{

	std::shared_ptr<FighterCharacter<PlayerController>> player;
	std::shared_ptr<FighterCharacter<DummyController>> enemy;

	switch(NETWORK.GetPlace())
	{
	case 1:
		player = GetWorld().SpawnActor<FighterCharacter<PlayerController>>(FVec2D(-250, -125), "Player", static_cast<FighterCharacter<PlayerController>::EFighterName>(selection.first - 1), 15);
		enemy = GetWorld().SpawnActor<FighterCharacter<DummyController>>(FVec2D(250, -125), "Enemy", static_cast<FighterCharacter<DummyController>::EFighterName>(selection.second - 1), 12);
		// Create UI Canvas
		CreateCanvas<UICanvas_SinglePlayerCombat>(player->GetFighter(), enemy->GetFighter());
		break;
	case 2:
		player = GetWorld().SpawnActor<FighterCharacter<PlayerController>>(FVec2D(250, -125), "Player", static_cast<FighterCharacter<PlayerController>::EFighterName>(selection.first - 1), 15);
		enemy = GetWorld().SpawnActor<FighterCharacter<DummyController>>(FVec2D(-250, -125), "Enemy", static_cast<FighterCharacter<DummyController>::EFighterName>(selection.second - 1), 12);
		// Create UI Canvas
		CreateCanvas<UICanvas_SinglePlayerCombat>(enemy->GetFighter(), player->GetFighter());
		break;
	default:
		break;
	}

	player->GetFighter()->SetEnemy(enemy->GetFighter());
	enemy->GetFighter()->SetEnemy(player->GetFighter());

	

	// Play background sounds
	SFX.Play("Sounds/BeJustOrBeDead.wav");
}

void MultiPlayerCombatLevel::SetSelection(std::pair<int, int> _selection)
{
	selection = _selection;
}

#include "GameEngine/Levels/MultiPlayerCombatLevel.h"

#include "GameEngine/Actors/Background.h"
#include "GameEngine/Controllers/DummyController.h"
#include "GameEngine/Actors/FighterCharacter.h"
#include "GameEngine/Controllers/NetworkEnemyController.h"
#include "NetworkEngine/NetworkSystem.h"
#include "GameEngine/Controllers/PlayerController.h"
#include "SoundEngine/SoundSystem.h"
#include "GameEngine/UI/UICanvas_SinglePlayerCombat.h"

MultiPlayerCombatLevel::MultiPlayerCombatLevel()
{
	GetWorld().SpawnActor<Background>(FVec2D(0, 180), "Background");
}

void MultiPlayerCombatLevel::Update()
{
	Level::Update();

	// Send Data
	SendPositionData(player->GetPosition());
}

void MultiPlayerCombatLevel::BeginLevel()
{
	switch(NETWORK.GetPlace())
	{
	case 1:
		player = GetWorld().SpawnActor<FighterCharacter<PlayerController>>(FVec2D(-250, -125), "Player", static_cast<EFighterName>(selection.first - 1), 15);
		enemy = GetWorld().SpawnActor<FighterCharacter<NetworkEnemyController>>(FVec2D(250, -125), "Enemy", static_cast<EFighterName>(selection.second - 1), 12);
		// Create UI Canvas
		CreateCanvas<UICanvas_SinglePlayerCombat>(player->GetFighter(), enemy->GetFighter());
		break;
	case 2:
		player = GetWorld().SpawnActor<FighterCharacter<PlayerController>>(FVec2D(250, -125), "Player", static_cast<EFighterName>(selection.first - 1), 15);
		enemy = GetWorld().SpawnActor<FighterCharacter<NetworkEnemyController>>(FVec2D(-250, -125), "Enemy", static_cast<EFighterName>(selection.second - 1), 12);
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

void MultiPlayerCombatLevel::SendPositionData(FVec2D position)
{
	std::vector<uint8_t> output;
	uint8_t* position_x = reinterpret_cast<uint8_t*>(&position.x);
	uint8_t* position_y = reinterpret_cast<uint8_t*>(&position.y);
	for (int i = 0; i < 4; i++)
		output.emplace_back(position_x[i]);
	for (int i = 0; i < 4; i++)
		output.emplace_back(position_y[i]);
	NETWORK.SendData(output);
}

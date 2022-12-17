#include "GameEngine/Levels/MultiPlayerCombatLevel.h"

#include "SoundEngine/SoundSystem.h"
#include "NetworkEngine/NetworkSystem.h"
#include "GameEngine/Actors/Background.h"
#include "GameEngine/Actors/FighterCharacter.h"
#include "GameEngine/Controllers/NetworkEnemyController.h"
#include "GameEngine/Controllers/PlayerController.h"
#include "GameEngine/Levels/VictoryLevel.h"
#include "GameEngine/UI/UICanvas_SinglePlayerCombat.h"
#include "MainEngine/Engine.h"


void MultiPlayerCombatLevel::Update()
{
	Level::Update();

	// Send Data
	SendPositionAndActionData(pPlayer->GetPosition(), std::dynamic_pointer_cast<FightingController>(pPlayer->GetController())->GetAction());

	if (!(pPlayer->GetFighter()->IsAlive() && pEnemy->GetFighter()->IsAlive()))
	{
		SFX.Pause("Sounds\\BeJustOrBeDead.wav");
		NETWORK.Disconnect();
		ENGINE.SetLevel<VictoryLevel>();
		if (std::shared_ptr<VictoryLevel> new_level = std::dynamic_pointer_cast<VictoryLevel>(ENGINE.GetCurrentLevel()))
		{
			new_level->SetWinner(*(pPlayer->GetFighter()->IsAlive() ? pPlayer->GetFighter() : pEnemy->GetFighter()),
				(pPlayer->GetFighter()->IsAlive() && NETWORK.GetPlace() == 1) ? 1 : (pEnemy->GetFighter()->IsAlive() && NETWORK.GetPlace() == 2) ? 1 : 2);
			new_level->BeginLevel();
		}
	}
}

void MultiPlayerCombatLevel::BeginLevel()
{
	// Clear level from previous access
	Clear();

	// Spawn Actors
	GetWorld().SpawnActor<Background>(FVec2D(0, 180), "Background");
	switch(NETWORK.GetPlace())
	{
	case 1:
		pPlayer = GetWorld().SpawnActor<FighterCharacter<PlayerController>>(FVec2D(-250, -125), "Player", static_cast<EFighterName>(selection.first - 1), 15);
		pEnemy = GetWorld().SpawnActor<FighterCharacter<NetworkEnemyController>>(FVec2D(250, -125), "Enemy", static_cast<EFighterName>(selection.second - 1), 12);
		// Create UI Canvas
		CreateCanvas<UICanvas_SinglePlayerCombat>(pPlayer->GetFighter(), pEnemy->GetFighter());
		break;
	case 2:
		pPlayer = GetWorld().SpawnActor<FighterCharacter<PlayerController>>(FVec2D(250, -125), "Player", static_cast<EFighterName>(selection.first - 1), 15);
		pEnemy = GetWorld().SpawnActor<FighterCharacter<NetworkEnemyController>>(FVec2D(-250, -125), "Enemy", static_cast<EFighterName>(selection.second - 1), 12);
		// Create UI Canvas
		CreateCanvas<UICanvas_SinglePlayerCombat>(pEnemy->GetFighter(), pPlayer->GetFighter());
		break;
	default:
		break;
	}

	// Linking Player and Enemy so they face each other
	pPlayer->GetFighter()->SetEnemy(pEnemy->GetFighter());
	pEnemy->GetFighter()->SetEnemy(pPlayer->GetFighter());

	// Play background sounds
	SFX.Play("Sounds\\BeJustOrBeDead.wav");
}

void MultiPlayerCombatLevel::SetSelection(std::pair<int, int> _selection)
{
	selection = _selection;
}

void MultiPlayerCombatLevel::SendPositionAndActionData(FVec2D position, FightingController::EAction action)
{
	// We reinterpret the floating point values as uint8_t arrays of size 4
	std::vector<uint8_t> output;
	uint8_t* position_x = reinterpret_cast<uint8_t*>(&position.x);
	uint8_t* position_y = reinterpret_cast<uint8_t*>(&position.y);
	for (int i = 0; i < 4; i++)
		output.emplace_back(position_x[i]);
	for (int i = 0; i < 4; i++)
		output.emplace_back(position_y[i]);
	output.emplace_back(static_cast<uint8_t>(action));
	NETWORK.SendData(output);
}

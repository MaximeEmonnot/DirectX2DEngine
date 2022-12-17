#include "GameEngine/Controllers/PlayerController.h"

#include "CoreEngine/Keyboard.h"
#include "MainEngine/Pawn.h"
#include "MainEngine/Collider.h"
#include "GameEngine/Commands.h"
#include "GameEngine/Actors/FighterCharacter.h"

PlayerController::PlayerController(Pawn& owner)
	:
	FightingController(owner)
{
}

void PlayerController::Update()
{
	action = EAction::None;

	if (KBD.KeyIsPressed(Commands::PUNCH)) action = EAction::Punch;
	if (KBD.KeyIsPressed(Commands::KICK)) action = EAction::Kick;
	if (KBD.KeyIsPressed(Commands::SLASH)) action = EAction::Slash;
	if (KBD.KeyIsPressed(Commands::HARDSLASH)) action = EAction::HardSlash;
	if (KBD.KeyIsPressed(Commands::DOWN)) action = EAction::Down;

	if (action == EAction::None) {

		FVec2D move_input;
		if (KBD.KeyIsPressed(Commands::RIGHT)) move_input.x++;
		if (KBD.KeyIsPressed(Commands::LEFT)) move_input.x--;

		if (bIsLookingLeft)
			action = (move_input.x < 0) ? EAction::Front : (move_input.x > 0) ? EAction::Back : EAction::None;
		else
			action = (move_input.x < 0) ? EAction::Back : (move_input.x > 0) ? EAction::Front : EAction::None;

		if (KBD.KeyIsPressed(Commands::JUMP)) {
			if (!dynamic_cast<Character*>(&owner)->GetRootCollider()->IsFalling()) dynamic_cast<Character*>(&owner)->GetRootCollider()->AddImpulse(FVec2D(0.f, 60.f));
			action = EAction::Jump;
		}

		dynamic_cast<Character*>(&owner)->GetRootCollider()->AddInputMovement(move_input * speed);
	}
}

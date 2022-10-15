#include "GameEngine/Controllers/PlayerController.h"

#include "CoreEngine/Keyboard.h"
#include "MainEngine/Pawn.h"
#include "MainEngine/Collider.h"
#include "GameEngine/Commands.h"
#include "GameEngine/Actors/FighterCharacter.h"

PlayerController::PlayerController(Pawn& owner)
	:
	Controller(owner)
{
}

void PlayerController::Update()
{
	FVec2D moveInput;
	if (KBD.KeyIsPressed(Commands::RIGHT)) moveInput.x++;
	if (KBD.KeyIsPressed(Commands::LEFT)) moveInput.x--;

	if (KBD.KeyIsPressed(Commands::JUMP))
		if (!dynamic_cast<Character*>(&owner)->GetRootCollider()->IsFalling()) dynamic_cast<Character*>(&owner)->GetRootCollider()->AddImpulse(FVec2D(0.f, 60.f));

	dynamic_cast<Character*>(&owner)->GetRootCollider()->AddInputMovement(moveInput * speed);
}

#include "PlayerController.h"

#include "Collider.h"
#include "PlayerCharacter.h"
#include "Keyboard.h"
#include "Pawn.h"
#include "Commands.h"

//**** PLAYER CONTROLLER ****//

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
		if (!dynamic_cast<PlayerCharacter*>(&owner)->rootCollider->IsFalling()) dynamic_cast<PlayerCharacter*>(&owner)->rootCollider->AddImpulse(FVec2D(0, 500));

	dynamic_cast<PlayerCharacter*>(&owner)->rootCollider->AddInputMovement(moveInput * speed);
}

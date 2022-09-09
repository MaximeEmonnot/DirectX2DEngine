#include "RoboKy.h"

#include "Commands.h"
#include "Keyboard.h"
#include "PlayerCharacter.h"

RoboKy::RoboKyComboTree::RoboKyComboTree()
{
}

RoboKy::RoboKy(Actor& owner)
	:
	BaseFighter(owner, "json/roboky.json", std::make_shared<RoboKyComboTree>())
{
	// WalkingForward
	animSys.AddTransition("Idle", "WalkingForward", [&] { return KBD.KeyIsPressed(Commands::LEFT); });
	animSys.AddTransition("RandomIdle", "WalkingForward", [&] { return KBD.KeyIsPressed(Commands::LEFT); });
	animSys.AddTransition("WalkingForward", "Idle", [&] { return !KBD.KeyIsPressed(Commands::LEFT); });

	// WalkingBack
	animSys.AddTransition("Idle", "WalkingBackward", [&] { return KBD.KeyIsPressed(Commands::RIGHT); });
	animSys.AddTransition("RandomIdle", "WalkingBackward", [&] { return KBD.KeyIsPressed(Commands::RIGHT); });
	animSys.AddTransition("WalkingBackward", "Idle", [&] { return !KBD.KeyIsPressed(Commands::RIGHT); });

	// Crouch
	animSys.AddTransition("Idle", "CrouchTransition", [&] { return KBD.KeyIsPressed(Commands::DOWN); });
	animSys.AddTransition("RandomIdle", "CrouchTransition", [&] { return KBD.KeyIsPressed(Commands::DOWN); });
	animSys.AddTransition("CrouchTransition", "CrouchIdle", [&] { return KBD.KeyIsPressed(Commands::DOWN) && animSys.AnimationIsFinished(); });
	animSys.AddTransition("CrouchIdle", "CrouchTransition", [&] { return !KBD.KeyIsPressed(Commands::DOWN); });
	animSys.AddTransition("CrouchTransition", "Idle", [&] {return !KBD.KeyIsPressed(Commands::DOWN) && animSys.AnimationIsFinished(); });

	// Punch
	animSys.AddTransition("Idle", "Punch", [&] { return KBD.KeyIsPressed(Commands::PUNCH); });
	animSys.AddTransition("RandomIdle", "Punch", [&] { return KBD.KeyIsPressed(Commands::PUNCH); });
	animSys.AddTransition("Punch", "Idle", [&] { return animSys.AnimationIsFinished(); });

	// Kick
	animSys.AddTransition("Idle", "Kick", [&] { return KBD.KeyIsPressed(Commands::KICK); });
	animSys.AddTransition("RandomIdle", "Kick", [&] { return KBD.KeyIsPressed(Commands::KICK); });
	animSys.AddTransition("Kick", "Idle", [&] { return animSys.AnimationIsFinished(); });

	// Slash
	animSys.AddTransition("Idle", "Slash", [&] { return KBD.KeyIsPressed(Commands::SLASH); });
	animSys.AddTransition("RandomIdle", "Slash", [&] { return KBD.KeyIsPressed(Commands::SLASH); });
	animSys.AddTransition("Slash", "Idle", [&] { return animSys.AnimationIsFinished(); });

	// HardSlash
	animSys.AddTransition("Idle", "HardSlash", [&] { return KBD.KeyIsPressed(Commands::HARDSLASH); });
	animSys.AddTransition("RandomIdle", "HardSlash", [&] { return KBD.KeyIsPressed(Commands::HARDSLASH); });
	animSys.AddTransition("HardSlash", "Idle", [&] { return animSys.AnimationIsFinished(); });

	// Jump
	animSys.AddTransition("Idle", "Jump", [&] { return KBD.KeyIsPressed(Commands::JUMP); });
	animSys.AddTransition("RandomIdle", "Jump", [&] { return KBD.KeyIsPressed(Commands::JUMP); });
	animSys.AddTransition("WalkingForward", "Jump", [&] { return KBD.KeyIsPressed(Commands::JUMP); });
	animSys.AddTransition("WalkingBackward", "Jump", [&] { return KBD.KeyIsPressed(Commands::JUMP); });
	animSys.AddTransition("Jump", "MaxJump", [&] {return animSys.AnimationIsFinished(); });
	animSys.AddTransition("MaxJump", "Fall", [&] { return animSys.AnimationIsFinished(); });


	// Fall
	animSys.AddTransition("Idle", "Fall", [&]
		{
			PlayerCharacter* player = dynamic_cast<PlayerCharacter*>(&owner);
			if (player) return !player->IsGrounded();
			return false;
		});
	animSys.AddTransition("RandomIdle", "Fall", [&]
		{
			PlayerCharacter* player = dynamic_cast<PlayerCharacter*>(&owner);
			if (player) return !player->IsGrounded();
			return false;
		});
	animSys.AddTransition("Fall", "Idle", [&]
		{
			PlayerCharacter* player = dynamic_cast<PlayerCharacter*>(&owner);
			if (player) return player->IsGrounded();
			return false;
		});
}

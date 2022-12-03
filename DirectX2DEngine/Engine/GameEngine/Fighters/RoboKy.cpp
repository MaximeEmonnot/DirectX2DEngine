#include "GameEngine/Fighters/RoboKy.h"

#include "CoreEngine/Keyboard.h"
#include "MainEngine/Character.h"
#include "GameEngine/Commands.h"

RoboKy::RoboKyComboTree::RoboKyComboTree()
{
}

RoboKy::RoboKy(Actor& owner, int priority)
	:
	BaseFighter(owner, "json/roboky.json", std::make_shared<RoboKyComboTree>(), 120, priority)
{
	// Name Initialization
	name = L"Robo Ky";

	// ANIMATIONS TRANSITIONS
	// WalkingForward
	AddAnimationTransition("Idle", "WalkingForward", [&] { return KBD.KeyIsPressed(Commands::LEFT); });
	AddAnimationTransition("RandomIdle", "WalkingForward", [&] { return KBD.KeyIsPressed(Commands::LEFT); });
	AddAnimationTransition("WalkingForward", "Idle", [&] { return !KBD.KeyIsPressed(Commands::LEFT); });

	// WalkingBack
	AddAnimationTransition("Idle", "WalkingBackward", [&] { return KBD.KeyIsPressed(Commands::RIGHT); });
	AddAnimationTransition("RandomIdle", "WalkingBackward", [&] { return KBD.KeyIsPressed(Commands::RIGHT); });
	AddAnimationTransition("WalkingBackward", "Idle", [&] { return !KBD.KeyIsPressed(Commands::RIGHT); });

	// Crouch
	AddAnimationTransition("Idle", "CrouchTransition", [&] { return KBD.KeyIsPressed(Commands::DOWN); });
	AddAnimationTransition("RandomIdle", "CrouchTransition", [&] { return KBD.KeyIsPressed(Commands::DOWN); });
	AddAnimationTransition("CrouchTransition", "CrouchIdle", [&] { return KBD.KeyIsPressed(Commands::DOWN) && (animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished()); });
	AddAnimationTransition("CrouchIdle", "CrouchTransition", [&] { return !KBD.KeyIsPressed(Commands::DOWN); });
	AddAnimationTransition("CrouchTransition", "Idle", [&] {return !KBD.KeyIsPressed(Commands::DOWN) && (animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished()); });

	// Punch
	AddAnimationTransition("Idle", "Punch", [&] { return KBD.KeyIsPressed(Commands::PUNCH); });
	AddAnimationTransition("RandomIdle", "Punch", [&] { return KBD.KeyIsPressed(Commands::PUNCH); });
	AddAnimationTransition("Punch", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

	// Kick
	AddAnimationTransition("Idle", "Kick", [&] { return KBD.KeyIsPressed(Commands::KICK); });
	AddAnimationTransition("RandomIdle", "Kick", [&] { return KBD.KeyIsPressed(Commands::KICK); });
	AddAnimationTransition("Kick", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

	// Slash
	AddAnimationTransition("Idle", "Slash", [&] { return KBD.KeyIsPressed(Commands::SLASH); });
	AddAnimationTransition("RandomIdle", "Slash", [&] { return KBD.KeyIsPressed(Commands::SLASH); });
	AddAnimationTransition("Slash", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

	// HardSlash
	AddAnimationTransition("Idle", "HardSlash", [&] { return KBD.KeyIsPressed(Commands::HARDSLASH); });
	AddAnimationTransition("RandomIdle", "HardSlash", [&] { return KBD.KeyIsPressed(Commands::HARDSLASH); });
	AddAnimationTransition("HardSlash", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

	// Jump
	AddAnimationTransition("Idle", "Jump", [&] { return KBD.KeyIsPressed(Commands::JUMP); });
	AddAnimationTransition("RandomIdle", "Jump", [&] { return KBD.KeyIsPressed(Commands::JUMP); });
	AddAnimationTransition("WalkingForward", "Jump", [&] { return KBD.KeyIsPressed(Commands::JUMP); });
	AddAnimationTransition("WalkingBackward", "Jump", [&] { return KBD.KeyIsPressed(Commands::JUMP); });
	AddAnimationTransition("Jump", "MaxJump", [&] {return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });
	AddAnimationTransition("MaxJump", "Fall", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

	// Fall
	AddAnimationTransition("Idle", "Fall", [&]
		{
			if (const Character* player = dynamic_cast<Character*>(&owner)) return !player->IsGrounded();
			return false;
		});
	AddAnimationTransition("RandomIdle", "Fall", [&]
		{
			if (const Character* player = dynamic_cast<Character*>(&owner)) return !player->IsGrounded();
			return false;
		});
	AddAnimationTransition("Fall", "Idle", [&]
		{
			if (const Character* player = dynamic_cast<Character*>(&owner)) return player->IsGrounded();
			return false;
		});
}

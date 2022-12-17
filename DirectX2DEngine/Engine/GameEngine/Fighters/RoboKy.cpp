#include "GameEngine/Fighters/RoboKy.h"

#include "CoreEngine/Keyboard.h"
#include "MainEngine/Character.h"
#include "GameEngine/Commands.h"

RoboKy::RoboKyComboTree::RoboKyComboTree()
{
}

RoboKy::RoboKy(Actor& owner, int priority)
	:
	BaseFighter(owner, "json\\roboky.json", std::make_shared<RoboKyComboTree>(), 120, priority)
{
	// Name Initialization
	name = L"Robo Ky";

	// ANIMATIONS TRANSITIONS
	// WalkingForward
	AddAnimationTransition("Idle", "WalkingForward", [&] { return pController->IsDoingAction(FightingController::EAction::Front); });
	AddAnimationTransition("RandomIdle", "WalkingForward", [&] { return pController->IsDoingAction(FightingController::EAction::Front); });
	AddAnimationTransition("WalkingForward", "Idle", [&] { return !pController->IsDoingAction(FightingController::EAction::Front); });

	// WalkingBack
	AddAnimationTransition("Idle", "WalkingBackward", [&] { return pController->IsDoingAction(FightingController::EAction::Back); });
	AddAnimationTransition("RandomIdle", "WalkingBackward", [&] { return pController->IsDoingAction(FightingController::EAction::Back); });
	AddAnimationTransition("WalkingBackward", "Idle", [&] { return !pController->IsDoingAction(FightingController::EAction::Back); });

	// Crouch
	AddAnimationTransition("Idle", "CrouchTransition", [&] { return pController->IsDoingAction(FightingController::EAction::Down); });
	AddAnimationTransition("RandomIdle", "CrouchTransition", [&] { return pController->IsDoingAction(FightingController::EAction::Down); });
	AddAnimationTransition("CrouchTransition", "CrouchIdle", [&] { return pController->IsDoingAction(FightingController::EAction::Down) && (animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished()); });
	AddAnimationTransition("CrouchIdle", "CrouchTransition", [&] { return !pController->IsDoingAction(FightingController::EAction::Down); });
	AddAnimationTransition("CrouchTransition", "Idle", [&] {return !pController->IsDoingAction(FightingController::EAction::Down) && (animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished()); });

	// Punch
	AddAnimationTransition("Idle", "Punch", [&] { return pController->IsDoingAction(FightingController::EAction::Punch); });
	AddAnimationTransition("RandomIdle", "Punch", [&] { return pController->IsDoingAction(FightingController::EAction::Punch); });
	AddAnimationTransition("Punch", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

	// Kick
	AddAnimationTransition("Idle", "Kick", [&] { return pController->IsDoingAction(FightingController::EAction::Kick); });
	AddAnimationTransition("RandomIdle", "Kick", [&] { return pController->IsDoingAction(FightingController::EAction::Kick); });
	AddAnimationTransition("Kick", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

	// Slash
	AddAnimationTransition("Idle", "Slash", [&] { return pController->IsDoingAction(FightingController::EAction::Slash); });
	AddAnimationTransition("RandomIdle", "Slash", [&] { return pController->IsDoingAction(FightingController::EAction::Slash); });
	AddAnimationTransition("Slash", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

	// HardSlash
	AddAnimationTransition("Idle", "HardSlash", [&] { return pController->IsDoingAction(FightingController::EAction::HardSlash); });
	AddAnimationTransition("RandomIdle", "HardSlash", [&] { return pController->IsDoingAction(FightingController::EAction::HardSlash); });
	AddAnimationTransition("HardSlash", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

	// Jump
	AddAnimationTransition("Idle", "Jump", [&] { return pController->IsDoingAction(FightingController::EAction::Jump); });
	AddAnimationTransition("RandomIdle", "Jump", [&] { return pController->IsDoingAction(FightingController::EAction::Jump); });
	AddAnimationTransition("WalkingForward", "Jump", [&] { return pController->IsDoingAction(FightingController::EAction::Jump); });
	AddAnimationTransition("WalkingBackward", "Jump", [&] { return pController->IsDoingAction(FightingController::EAction::Jump); });
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

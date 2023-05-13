#include "GameEngine/Fighters/SolBadguy.h"

#include <random>

#include "CoreEngine/Keyboard.h"
#include "MainEngine/Character.h"
#include "GameEngine/Commands.h"
#include "GameEngine/Controllers/FightingController.h"
#include "GameEngine/Controllers/FightingController.h"
#include "GameEngine/Controllers/FightingController.h"

SolBadguy::SolComboTree::SolComboTree()
{
	/// COMBOTREE INITIALIZATION
	// Down
	pRoot->leaves.emplace_back(std::make_shared<ComboNode>(Action::Down, "Down"));
	// Down DownRight Right
	pRoot->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownRight));
	pRoot->leaves.at(0)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Right, "Quarter Circle Front"));
	// Down DownLeft Left
	pRoot->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownLeft));
	pRoot->leaves.at(0)->leaves.at(1)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Left, "Quarter Circle Back"));
	// Down DownLeft Left Down DownLeft Left
	pRoot->leaves.at(0)->leaves.at(1)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Down));
	pRoot->leaves.at(0)->leaves.at(1)->leaves.at(0)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownLeft));
	pRoot->leaves.at(0)->leaves.at(1)->leaves.at(0)->leaves.at(0)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Left, "Double Quarter Circle Back"));

	// Left Left
	pRoot->leaves.emplace_back(std::make_shared<ComboNode>(Action::Left));
	pRoot->leaves.at(1)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Left, "Escape"));
	// Left Right
	pRoot->leaves.at(1)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Right));
	// Left DownLeft Down DownRight Right
	pRoot->leaves.at(1)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownLeft));
	pRoot->leaves.at(1)->leaves.at(2)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Down));
	pRoot->leaves.at(1)->leaves.at(2)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownRight));
	pRoot->leaves.at(1)->leaves.at(2)->leaves.at(0)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Right, "Half Circle Front"));

	// Right
	pRoot->leaves.emplace_back(std::make_shared<ComboNode>(Action::Right, "Right"));
	// Right Right
	pRoot->leaves.at(2)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Right, "Dash"));
	// Right Down DownRight
	pRoot->leaves.at(2)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Down));
	pRoot->leaves.at(2)->leaves.at(1)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownRight, "Dragon Punch"));
	// Right DownRight Down DownLeft Left Right
	pRoot->leaves.at(2)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownRight));
	pRoot->leaves.at(2)->leaves.at(2)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Down));
	pRoot->leaves.at(2)->leaves.at(2)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownLeft));
	pRoot->leaves.at(2)->leaves.at(2)->leaves.at(0)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Left));
	pRoot->leaves.at(2)->leaves.at(2)->leaves.at(0)->leaves.at(0)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Right, "Overdrive"));
}

SolBadguy::SolBadguy(Actor& owner, int priority)
	:
	BaseFighter(owner, "json\\sol.json", std::make_shared<SolComboTree>(), 140, priority)
{
	// Name initialization
	name = L"Sol Badguy";

	/// TRANSITIONS
	// Idle - RandomIdle
	AddAnimationTransition("Idle", "RandomIdle", [&] { return bIdleToRandomIdle; });
	AddAnimationTransition("RandomIdle", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

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

void SolBadguy::Update()
{
	// We update the condition for the transition from Idle to RandomIdle BEFORE the parent class Update()
	// so that the transition will occur during the current frame and not during the next frame
	IdleToRandomIdleConditionUpdate();

	BaseFighter::Update();
}

void SolBadguy::IdleToRandomIdleConditionUpdate()
{
	if (animSys.AnimationIsFinished()) {
		std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<int> dist(0, 500);
		bIdleToRandomIdle = dist(rng) == 400;
	}
	else bIdleToRandomIdle = false;
}


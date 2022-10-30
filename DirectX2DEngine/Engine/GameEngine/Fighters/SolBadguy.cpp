#include "GameEngine/Fighters/SolBadguy.h"

#include <random>

#include "CoreEngine/Keyboard.h"
#include "MainEngine/Character.h"
#include "GameEngine/Commands.h"

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
	BaseFighter(owner, "json/sol.json", std::make_shared<SolComboTree>(), priority)
{
	// Name initialization
	name = L"Sol Badguy";

	/// TRANSITIONS
	// Idle - RandomIdle
	AddAnimationTransition("Idle", "RandomIdle", [&] { return bIdleToRandomIdle; });
	AddAnimationTransition("RandomIdle", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

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
		const std::uniform_int_distribution<int> dist(0, 500);
		bIdleToRandomIdle = dist(rng) == 400;
	}
	else bIdleToRandomIdle = false;
}


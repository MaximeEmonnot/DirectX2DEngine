#include "GameEngine/SolBadguy.h"
#include "GameEngine/Commands.h"

#include <random>

#include "MainEngine/Character.h"
#include "CoreEngine/Keyboard.h"

SolBadguy::SolComboTree::SolComboTree()
{
	// Down
	root->leaves.emplace_back(std::make_shared<ComboNode>(Action::Down, "Down"));
	// Down DownRight Right
	root->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownRight));
	root->leaves.at(0)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Right, "Quarter Circle Front"));
	// Down DownLeft Left
	root->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownLeft));
	root->leaves.at(0)->leaves.at(1)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Left, "Quarter Circle Back"));
	// Down DownLeft Left Down DownLeft Left
	root->leaves.at(0)->leaves.at(1)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Down));
	root->leaves.at(0)->leaves.at(1)->leaves.at(0)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownLeft));
	root->leaves.at(0)->leaves.at(1)->leaves.at(0)->leaves.at(0)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Left, "Double Quarter Circle Back"));

	// Left Left
	root->leaves.emplace_back(std::make_shared<ComboNode>(Action::Left));
	root->leaves.at(1)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Left, "Escape"));
	// Left Right
	root->leaves.at(1)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Right));
	// Left DownLeft Down DownRight Right
	root->leaves.at(1)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownLeft));
	root->leaves.at(1)->leaves.at(2)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Down));
	root->leaves.at(1)->leaves.at(2)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownRight));
	root->leaves.at(1)->leaves.at(2)->leaves.at(0)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Right, "Half Circle Front"));

	// Right
	root->leaves.emplace_back(std::make_shared<ComboNode>(Action::Right, "Right"));
	// Right Right
	root->leaves.at(2)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Right, "Dash"));
	// Right Down DownRight
	root->leaves.at(2)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Down));
	root->leaves.at(2)->leaves.at(1)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownRight, "Dragon Punch"));
	// Right DownRight Down DownLeft Left Right
	root->leaves.at(2)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownRight));
	root->leaves.at(2)->leaves.at(2)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Down));
	root->leaves.at(2)->leaves.at(2)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownLeft));
	root->leaves.at(2)->leaves.at(2)->leaves.at(0)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Left));
	root->leaves.at(2)->leaves.at(2)->leaves.at(0)->leaves.at(0)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Right, "Overdrive"));
}

SolBadguy::SolBadguy(Actor& owner, int priority)
	:
	BaseFighter(owner, "json/sol.json", std::make_shared<SolComboTree>(), priority)
{
	// Name initialization
	name = L"Sol Badguy";

	/// ANIMATIONS TRANSITIONS
	// Idle - RandomIdle
	animSys.AddTransition("Idle", "RandomIdle", [&]
		{
			if (animSys.AnimationIsFinished()) {
				std::mt19937 rng(std::random_device{}());
				const std::uniform_int_distribution<int> dist(0, 500);
				return dist(rng) == 400;
			}
			return false;
		});
	animSys.AddTransition("RandomIdle", "Idle", [&] {return animSys.AnimationIsFinished(); });

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
			Character* player = dynamic_cast<Character*>(&owner);
			if (player) return !player->IsGrounded();
			return false;
		});
	animSys.AddTransition("RandomIdle", "Fall", [&]
		{
			Character* player = dynamic_cast<Character*>(&owner);
			if (player) return !player->IsGrounded();
			return false;
		});
	animSys.AddTransition("Fall", "Idle", [&]
		{
			Character* player = dynamic_cast<Character*>(&owner);
			if (player) return player->IsGrounded();
			return false;
		});

	///COLLISIONS TRANSITIONS
	collisionSys.AddTransition("Idle", "RandomIdle", [&]
		{
			return false;
		});
}


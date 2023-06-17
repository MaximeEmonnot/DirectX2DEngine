export module GameEngine:Controllers;

import MainEngine;
import NetworkEngine;
import MathEngine;
import CoreEngine;

import GameEngine;

export
class FightingController : public Controller
{
public:
	enum class EAction
	{
		Front,
		Back,
		Down,
		Jump,
		Punch,
		Kick,
		Slash,
		HardSlash,
		None
	};
public:
	FightingController(Pawn& owner)
		:
		Controller(owner)
	{}

	void SetLookAtDirection(bool bValue)
	{
		bIsLookingLeft = bValue;
	}

	bool IsDoingAction(const EAction& actionTest) const
	{
		return action == actionTest;
	}
	EAction GetAction() const
	{
		return action;
	}

protected:
	EAction action = EAction::None;

	bool bIsLookingLeft = true;
};

export
class DummyController :
	public FightingController
{
public:
	DummyController(Pawn& owner)
		:
		FightingController(owner)
	{}

	void Update() override {}
};


export
class NetworkEnemyController :
	public FightingController
{
public:
	NetworkEnemyController(Pawn& owner)
		:
		FightingController(owner),
		lastPosition(owner.GetPosition())
	{}

	void	Update() override
	{
		if (WND.IsPlaying() && NETWORK.IsOnline())
		{
			const std::vector<uint8_t> input = NETWORK.ReceiveDataUDP();
			// Reading X-Y position from vector, we will reinterpret them as floating point values
			if (!input.empty() && input.at(1) != NETWORK.GetPlace()) {
				uint8_t position_x[4] = { input.at(2), input.at(3), input.at(4), input.at(5) };
				uint8_t position_y[4] = { input.at(6), input.at(7), input.at(8), input.at(9) };

				lastPosition = FVec2D(*reinterpret_cast<float*>(&position_x), *reinterpret_cast<float*>(&position_y));
				action = static_cast<EAction>(input.at(10));
			}
		}

		owner.SetPosition(lastPosition);
	}
private:
	FVec2D	lastPosition;
};

export
class PlayerController :
	public FightingController
{
public:
	PlayerController(Pawn& owner)
		:
		FightingController(owner)
	{}

	void    Update() override
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

private:
	float   speed = 100.0f;
};


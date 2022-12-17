#pragma once
#include "MainEngine/Controller.h"

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
	FightingController(class Pawn& owner);

	void SetLookAtDirection(bool bValue);

	bool IsDoingAction(const EAction& actionTest) const;

protected:
	EAction action = EAction::None;

	bool bIsLookingLeft = true;
};

#pragma once
class Controller
{
public:
	Controller(class Pawn& owner);

	virtual void Update() = 0;

protected:
	class Pawn& owner;
};


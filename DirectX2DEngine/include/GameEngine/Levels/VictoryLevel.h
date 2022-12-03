#pragma once

#include "MainEngine/Level.h"

#include "GameEngine/Fighters/BaseFighter.h"

class VictoryLevel :
	public Level
{
public:
	VictoryLevel() = default;

    virtual void    Update() override;
    virtual void    BeginLevel() override;

	void			SetWinner(const BaseFighter& newWinnerFighter, int newPlayerNumber);

private:
	std::string		winnerFighter;
	int				playerNumber;
};
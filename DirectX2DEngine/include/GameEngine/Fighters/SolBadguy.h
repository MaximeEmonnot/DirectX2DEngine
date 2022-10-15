#pragma once

#include "BaseFighter.h"

class SolBadguy :
    public BaseFighter
{
private:
	class SolComboTree : public ComboTree
	{
	public:
		SolComboTree();
	};

public:
	SolBadguy(Actor& owner, int priority);
};


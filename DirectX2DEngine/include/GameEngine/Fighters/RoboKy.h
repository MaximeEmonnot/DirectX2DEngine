#pragma once
#include "BaseFighter.h"
class RoboKy : public BaseFighter
{
private:
	class RoboKyComboTree : public ComboTree
	{
	public:
		RoboKyComboTree();
	};
public:
	RoboKy(Actor& owner, int priority);
};


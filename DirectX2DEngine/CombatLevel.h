#pragma once
#include "DummyController.h"
#include "FighterCharacter.h"
#include "Level.h"
#include "PlayerController.h"
#include "SolBadguy.h"

class CombatLevel :
    public Level
{
public:
    CombatLevel();

    virtual void Update() override;
    virtual void BeginLevel() override;
};


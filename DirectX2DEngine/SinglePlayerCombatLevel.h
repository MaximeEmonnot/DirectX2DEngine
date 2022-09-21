#pragma once
#include "DummyController.h"
#include "FighterCharacter.h"
#include "Level.h"
#include "PlayerController.h"
#include "SolBadguy.h"

class SinglePlayerCombatLevel :
    public Level
{
public:
    SinglePlayerCombatLevel();

    virtual void Update() override;
    virtual void BeginLevel() override;

    void SetSelection(std::pair<int, int> _selection);
private:
    std::pair<int, int> selection;
};


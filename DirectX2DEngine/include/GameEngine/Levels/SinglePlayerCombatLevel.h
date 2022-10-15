#pragma once
#include "GameEngine/Controllers/DummyController.h"
#include "GameEngine/Actors/FighterCharacter.h"
#include "MainEngine/Level.h"
#include "GameEngine/Controllers/PlayerController.h"
#include "GameEngine/Fighters/SolBadguy.h"

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


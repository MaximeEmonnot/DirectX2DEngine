#pragma once

#include "MainEngine/Level.h"

#include "GameEngine/Actors/FighterCharacter.h"
#include "GameEngine/Controllers/DummyController.h"
#include "GameEngine/Controllers/PlayerController.h"

class SinglePlayerCombatLevel :
    public Level
{
public:
    SinglePlayerCombatLevel() = default;

    virtual void    Update() override;
    virtual void    BeginLevel() override;

    void            SetSelection(std::pair<int, int> _selection);
private:
    std::pair<int, int>     selection;

    std::shared_ptr<FighterCharacter<PlayerController>> pPlayer;
    std::shared_ptr<FighterCharacter<DummyController>> pEnemy;
};


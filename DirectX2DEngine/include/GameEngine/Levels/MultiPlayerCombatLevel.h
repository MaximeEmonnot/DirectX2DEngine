#pragma once

#include "MainEngine/Level.h"
#include "GameEngine/Actors/FighterCharacter.h"
#include "GameEngine/Controllers/PlayerController.h"
#include "GameEngine/Controllers/NetworkEnemyController.h"

class MultiPlayerCombatLevel :
    public Level
{
public:
    MultiPlayerCombatLevel();

    virtual void    Update() override;
    virtual void    BeginLevel() override;

    void            SetSelection(std::pair<int, int> _selection);

private:
    void            SendPositionData(FVec2D position);

private:
    std::shared_ptr<FighterCharacter<PlayerController>>         pPlayer;
    std::shared_ptr<FighterCharacter<NetworkEnemyController>>   pEnemy;

    std::pair<int, int>                                         selection;
};


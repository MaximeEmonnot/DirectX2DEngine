#pragma once
#include "MainEngine/Level.h"
#include "FighterCharacter.h"
#include "NetworkEnemyController.h"
#include "PlayerController.h"

class MultiPlayerCombatLevel :
    public Level
{
public:
    MultiPlayerCombatLevel();

    virtual void Update() override;
    virtual void BeginLevel() override;

    void SetSelection(std::pair<int, int> _selection);

private:
    void SendPositionData(FVec2D position);

private:
    std::shared_ptr<FighterCharacter<PlayerController>> player;
    std::shared_ptr<FighterCharacter<NetworkEnemyController>> enemy;

    std::pair<int, int> selection;
};


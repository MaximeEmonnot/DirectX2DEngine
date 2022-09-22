#pragma once
#include "Level.h"
class MultiPlayerCombatLevel :
    public Level
{
public:
    MultiPlayerCombatLevel();

    virtual void Update() override;
    virtual void BeginLevel() override;

    void SetSelection(std::pair<int, int> _selection);
private:
    std::pair<int, int> selection;
};


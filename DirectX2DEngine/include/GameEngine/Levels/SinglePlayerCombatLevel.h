#pragma once

#include "MainEngine/Level.h"

class SinglePlayerCombatLevel :
    public Level
{
public:
    SinglePlayerCombatLevel();

    virtual void    Update() override;
    virtual void    BeginLevel() override;

    void            SetSelection(std::pair<int, int> _selection);
private:
    std::pair<int, int>     selection;
};


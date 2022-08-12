#pragma once
#include "Level.h"
class CombatLevel :
    public Level
{
public:
    CombatLevel();

    virtual void Update() override;
    virtual void Render() override;
};


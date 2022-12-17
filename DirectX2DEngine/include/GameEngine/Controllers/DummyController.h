#pragma once

#include "FightingController.h"

class DummyController :
    public FightingController
{
public:
    DummyController(Pawn& owner);

    void Update() override;
};


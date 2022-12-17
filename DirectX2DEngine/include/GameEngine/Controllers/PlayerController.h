#pragma once

#include "FightingController.h"

class PlayerController :
    public FightingController
{
public:
    PlayerController(class Pawn& owner);

    void    Update() override;

private:
    float   speed = 100.0f;
};


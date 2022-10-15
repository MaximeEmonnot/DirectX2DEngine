#pragma once

#include "MainEngine/Controller.h"

class DummyController :
    public Controller
{
public:
    DummyController(Pawn& owner);

    void Update() override;
};


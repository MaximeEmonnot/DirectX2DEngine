#pragma once
#include "MainEngine/Controller.h"

class PlayerController :
    public Controller
{
public:
    PlayerController(class Pawn& owner);

    void Update() override;

private:
    float speed = 100.0f;
};


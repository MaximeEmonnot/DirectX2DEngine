#pragma once
#include "Actor.h"
#include "Controller.h"

class Pawn :
    public Actor
{
public:
    Pawn() = delete;

    Pawn(const FVec2D& pos, const std::string& name, std::shared_ptr<Controller> pController);
    Pawn(const Pawn&) = default;
    Pawn& operator= (const Pawn& toCopy);
    virtual ~Pawn() override = default;

    virtual void Update() override;
    virtual void Render() override;

protected:
    std::shared_ptr<Controller> pController;
};


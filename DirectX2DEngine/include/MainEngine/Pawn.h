#pragma once

#include "Actor.h"
#include "Controller.h"

class Pawn :
    public Actor
{
public:
    Pawn() = delete;

    Pawn(World& world, const FVec2D& pos, const std::string& name, std::shared_ptr<Controller> pController);
    Pawn(const Pawn&) = default;
    Pawn& operator= (const Pawn& toCopy);
    virtual ~Pawn() override = default;

    virtual void    Update() override;

    std::shared_ptr<Controller> GetController() const;

protected:
    std::shared_ptr<Controller>     pController;
};


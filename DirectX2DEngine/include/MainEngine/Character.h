#pragma once
#include "Pawn.h"
class Character :
    public Pawn
{
public:
    Character() = delete;
    Character(World& world, const FVec2D& pos, const std::string& name, std::shared_ptr<Controller> pController);

    virtual void Update() override;

    std::shared_ptr<Collider> GetRootCollider() const;

    bool IsGrounded() const;

protected:
    std::shared_ptr<Collider> rootCollider;
};


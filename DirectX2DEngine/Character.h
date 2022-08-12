#pragma once
#include "Pawn.h"
class Character :
    public Pawn
{
public:
    Character() = delete;
    Character(const FVec2D& pos, const std::string& name, std::shared_ptr<Controller> pController);

    virtual void Update() override;
    virtual void Render() override;

protected:
    std::shared_ptr<Collider> rootCollider;
};


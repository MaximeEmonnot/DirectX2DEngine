#pragma once

#include "MainEngine/Actor.h"
#include "MainEngine/Collider.h"

class SandBag :
    public Actor
{
public:
    SandBag(World& world, const FRect& pos, const std::string& name);

    void Update() override;

    std::vector<std::shared_ptr<Collider>> GetColliders() const override;

private:
    std::shared_ptr<Collider> rootCollider;
    std::shared_ptr<Collider> defCollider;
    std::shared_ptr<ColorModel> model;
};


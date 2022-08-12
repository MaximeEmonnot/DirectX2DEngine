#pragma once
#include "BaseFighter.h"
#include "Character.h"
class PlayerCharacter :
    public Character
{
private:
    friend class PlayerController;
public:
    PlayerCharacter() = delete;
    PlayerCharacter(const FVec2D& pos, const std::string& name);

    virtual void Update() override;
    virtual void Render() override;

    virtual std::vector<std::shared_ptr<Collider>> GetColliders() const override;

    bool IsGrounded() const;
private:
    std::unique_ptr<BaseFighter> pFighter;
};


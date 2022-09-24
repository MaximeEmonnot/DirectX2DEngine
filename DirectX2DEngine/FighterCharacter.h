#pragma once
#include "BaseFighter.h"
#include "Character.h"
#include "Collider.h"
#include "RoboKy.h"
#include "SolBadguy.h"

enum class EFighterName
{
    SolBadguy,
    RoboKy
};

template<class ControllerClass>
class FighterCharacter : public Character
{
public:
    FighterCharacter() = delete;
    FighterCharacter(World& world, const FVec2D& pos, const std::string& name, EFighterName fighter_name, int priority)
	    :
		Character(world, pos, name, std::make_shared<ControllerClass>(*this))
    {
        switch(fighter_name)
        {
        case EFighterName::SolBadguy: pFighter = std::make_shared<SolBadguy>(*this, priority);
            break;
        case EFighterName::RoboKy: pFighter = std::make_shared<RoboKy>(*this, priority);
            break;
        default:
            break;
        }

        rootCollider->SetRectPos(FRect(0, 5, 10, 10));
        rootCollider->SetCollisionMode(Collider::CollisionMode::Blocking);
        rootCollider->SetGravity(true);
        rootCollider->SetVisible(true);
        rootCollider->SetCollisionChannel(Collider::CollisionChannel::Gravity);
    }

    virtual void Update() override
    {
        Character::Update();

        pFighter->Update();
    }

    virtual std::vector<std::shared_ptr<Collider>> GetColliders() const override
    {
        std::vector<std::shared_ptr<Collider>> out;
        out.emplace_back(rootCollider);
        return out;
    }

    std::shared_ptr<BaseFighter> GetFighter()
    {
        return pFighter;
    }

private:
    std::shared_ptr<BaseFighter> pFighter;
};


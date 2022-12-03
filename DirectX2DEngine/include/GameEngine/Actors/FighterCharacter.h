#pragma once

#include "MainEngine/Character.h"
#include "MainEngine/Collider.h"
#include "GameEngine/Fighters/BaseFighter.h"
#include "GameEngine/Fighters/SolBadguy.h"
#include "GameEngine/Fighters/RoboKy.h"

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
    FighterCharacter(World& world, const FVec2D& pos, const std::string& name, EFighterName fighterName, int priority)
	    :
		Character(world, pos, name, std::make_shared<ControllerClass>(*this))
    {
        switch(fighterName)
        {
        case EFighterName::SolBadguy: pFighter = std::make_shared<SolBadguy>(*this, priority);
            break;
        case EFighterName::RoboKy: pFighter = std::make_shared<RoboKy>(*this, priority);
            break;
        default:
            break;
        }

        pRootCollider->SetRectPos(FRect(0, 5, 10, 10));
        pRootCollider->SetCollisionMode(Collider::CollisionMode::Blocking);
        pRootCollider->SetGravity(true);
        pRootCollider->SetVisible(true);
        pRootCollider->SetCollisionChannel(Collider::CollisionChannel::Gravity);
    }

    virtual void Update() override
    {
        Character::Update();

        pFighter->Update();
    }

    virtual void ApplyDamage(const Actor& damageCauser, int damage) override
    {
        pFighter->ApplyDamage(damage);
    }

    virtual std::vector<std::shared_ptr<Collider>> GetColliders() const override
    {
        std::vector<std::shared_ptr<Collider>> out;
        out.emplace_back(pRootCollider);
        for (std::shared_ptr<Collider> pCollider : pFighter->GetColliders())
            out.emplace_back(pCollider);
        return out;
    }

    std::shared_ptr<BaseFighter> GetFighter()
    {
        return pFighter;
    }

private:
    std::shared_ptr<BaseFighter> pFighter;
};


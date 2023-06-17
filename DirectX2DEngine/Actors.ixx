module;
#include <DirectXMath.h>

export module GameEngine:Actors;

import <memory>;
import <string>;
import <vector>;

import GraphicsEngine;
import MainEngine;
import MathEngine;

export
class Background : public Actor
{
public:
	Background(World& world, const FVec2D& pos, const std::string& name)
		:
		Actor(world, pos, name),
		pCollider(std::make_shared<Collider>(*this)),
		pModel(GetWorld().CreateModel<TextureModel>(1, "Textures\\Backgrounds\\London.tga"))
	{
		pCollider->SetVisible(true);
		pCollider->SetGravity(false);
		pCollider->SetCollisionChannel(Collider::CollisionChannel::Gravity);
		pCollider->SetCollisionMode(Collider::CollisionMode::Blocking);
		pCollider->SetRectPos(FRect(0, -500, 2000, 75));

		pModel->SetPosition(pos);
	}

	virtual void									Update() override
	{
		Actor::Update();
		pCollider->Update();
	}

	virtual std::vector<std::shared_ptr<Collider>>	GetColliders() const override
	{
		std::vector<std::shared_ptr<Collider>> out;
		out.emplace_back(pCollider);
		return out;
	}
private:
	std::shared_ptr<TextureModel>	pModel;
	std::shared_ptr<Collider>		pCollider;
};



export
enum class EFighterName
{
    SolBadguy,
    RoboKy
};

export
template<class ControllerClass>
class FighterCharacter : public Character
{
public:
    FighterCharacter() = delete;
    FighterCharacter(World& world, const FVec2D& pos, const std::string& name, EFighterName fighterName, int priority)
        :
        Character(world, pos, name, std::make_shared<ControllerClass>(*this))
    {
        switch (fighterName)
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

export
class SandBag :
	public Actor
{
public:
	SandBag(World& world, const FRect& pos, const std::string& name)
		:
		Actor(world, pos.pos, name),
		pRootCollider(std::make_shared<Collider>(*this)),
		pDefenseCollider(std::make_shared<Collider>(*this, pos - pos.pos)),
		pModel(GetWorld().CreateModel<ColorModel>(15, pos, DirectX::XMFLOAT4(1.f, 1.f, 0.f, 1.f)))
	{
		pRootCollider->SetGravity(true);
		pRootCollider->SetVisible(true);
		pRootCollider->SetCollisionMode(Collider::CollisionMode::Blocking);
		pRootCollider->SetCollisionChannel(Collider::CollisionChannel::Gravity);
		pRootCollider->SetRectPos(FRect(0, -96, 32, 32));

		pDefenseCollider->SetVisible(true);
		pDefenseCollider->SetCollisionChannel(Collider::CollisionChannel::Defense);
		pDefenseCollider->SetCollisionMode(Collider::CollisionMode::Overlapping);
	}

	void                                    Update() override
	{
		pRootCollider->Update();
		pDefenseCollider->Update();

		pModel->SetPosition(pos);
	}

	std::vector<std::shared_ptr<Collider>>  GetColliders() const override
	{
		std::vector<std::shared_ptr<Collider>> out;
		out.emplace_back(pRootCollider);
		out.emplace_back(pDefenseCollider);
		return out;
	}

private:
	std::shared_ptr<ColorModel>     pModel;
	std::shared_ptr<Collider>       pRootCollider;
	std::shared_ptr<Collider>       pDefenseCollider;
};
#include "GameEngine/Actors/SandBag.h"

#include "MainEngine/Engine.h"

SandBag::SandBag(World& world, const FRect& pos, const std::string& name)
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

void SandBag::Update()
{
	pRootCollider->Update();
	pDefenseCollider->Update();

	pModel->SetPosition(pos);
}

std::vector<std::shared_ptr<Collider>> SandBag::GetColliders() const
{
	std::vector<std::shared_ptr<Collider>> out;
	out.emplace_back(pRootCollider);
	out.emplace_back(pDefenseCollider);
	return out;
}

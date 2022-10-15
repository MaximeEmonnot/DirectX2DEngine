#include "GameEngine/Actors/SandBag.h"

#include "MainEngine/Engine.h"
#include "GraphicsEngine/Graphics.h"

SandBag::SandBag(World& world, const FRect& pos, const std::string& name)
	:
	Actor(world, pos.pos, name),
	rootCollider(std::make_shared<Collider>(*this)),
	defCollider(std::make_shared<Collider>(*this, pos - pos.pos)),
	model(GetWorld().CreateModel<ColorModel>(15, pos, DirectX::XMFLOAT4(1.f, 1.f, 0.f, 1.f)))
{
	rootCollider->SetGravity(true);
	rootCollider->SetVisible(true);
	rootCollider->SetCollisionMode(Collider::CollisionMode::Blocking);
	rootCollider->SetCollisionChannel(Collider::CollisionChannel::Gravity);

	rootCollider->SetRectPos(FRect(0, -96, 32, 32));

	defCollider->SetVisible(true);
	defCollider->SetCollisionChannel(Collider::CollisionChannel::Defense);
	defCollider->SetCollisionMode(Collider::CollisionMode::Overlapping);

}

void SandBag::Update()
{
	rootCollider->Update();
	defCollider->Update();

	model->SetPosition(pos);
}

std::vector<std::shared_ptr<Collider>> SandBag::GetColliders() const
{
	std::vector<std::shared_ptr<Collider>> out;
	out.emplace_back(rootCollider);
	out.emplace_back(defCollider);
	return out;
}

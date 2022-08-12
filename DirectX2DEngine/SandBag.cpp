#include "SandBag.h"
#include "Graphics.h"

SandBag::SandBag(const FRect& pos, const std::string& name)
	:
	Actor(pos.pos, name),
	rootCollider(std::make_shared<Collider>(*this)),
	defCollider(std::make_shared<Collider>(*this, pos - pos.pos)),
	model(pos, DirectX::XMFLOAT4(1.f, 1.f, 0.f, 1.f))
{
	rootCollider->SetGravity(true);
	rootCollider->SetCollisionMode(Collider::CollisionMode::Blocking);
	rootCollider->SetCollisionChannel(Collider::CollisionChannel::Gravity);
	rootCollider->SetVisible(true);
	rootCollider->SetRectPos(FRect(0, -96, 32, 32));

	defCollider->SetCollisionChannel(Collider::CollisionChannel::Defense);
	defCollider->SetCollisionMode(Collider::CollisionMode::Overlapping);
	defCollider->SetVisible(true);

	model.Initialize();
}

void SandBag::Update()
{
	rootCollider->Update();
	defCollider->Update();

	model.SetPosition(pos);
}

void SandBag::Render()
{
	model.Render();

	rootCollider->Render();
	defCollider->Render();
}

std::vector<std::shared_ptr<Collider>> SandBag::GetColliders() const
{
	std::vector<std::shared_ptr<Collider>> out;
	out.emplace_back(rootCollider);
	out.emplace_back(defCollider);
	return out;
}
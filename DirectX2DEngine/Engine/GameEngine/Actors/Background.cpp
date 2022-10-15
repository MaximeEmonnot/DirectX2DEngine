#include "GameEngine/Actors/Background.h"

#include "MainEngine/Engine.h"

Background::Background(World& world, const FVec2D& pos, const std::string& name)
	:
	Actor(world, pos, name),
	pCollider(std::make_shared<Collider>(*this)),
	pModel(GetWorld().CreateModel<TextureModel>(1, "Textures/Backgrounds/London.tga"))
{
	pCollider->SetVisible(true);
	pCollider->SetGravity(false);
	pCollider->SetCollisionChannel(Collider::CollisionChannel::Gravity);
	pCollider->SetCollisionMode(Collider::CollisionMode::Blocking);
	pCollider->SetRectPos(FRect(0, -500, 2000, 75));

	pModel->SetPosition(pos);
}

void Background::Update()
{
	Actor::Update();
	pCollider->Update();
}

std::vector<std::shared_ptr<Collider>> Background::GetColliders() const
{
	std::vector<std::shared_ptr<Collider>> out;
	out.emplace_back(pCollider);
	return out;
}

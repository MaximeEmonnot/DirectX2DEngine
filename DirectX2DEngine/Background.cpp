#include "Background.h"

#include "Engine.h"
#include "Graphics.h"

Background::Background(const FVec2D& pos, const std::string& name)
	:
	Actor(pos, name),
	collider(std::make_shared<Collider>(*this)),
	model(ENGINE.CreateModel<TextureModel>(32, "Textures/Backgrounds/China.tga"))
{
	collider->SetCollisionChannel(Collider::CollisionChannel::Gravity);
	collider->SetCollisionMode(Collider::CollisionMode::Blocking);
	collider->SetRectPos(FRect(0, -500, 2000, 100));
	collider->SetVisible(true);
	collider->SetGravity(false);
}

void Background::Update()
{
	Actor::Update();
	collider->Update();
}

std::vector<std::shared_ptr<Collider>> Background::GetColliders() const
{
	std::vector<std::shared_ptr<Collider>> out;
	out.emplace_back(collider);
	return out;
}

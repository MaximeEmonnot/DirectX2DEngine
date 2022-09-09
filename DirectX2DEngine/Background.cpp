#include "Background.h"

#include "Engine.h"
#include "Graphics.h"

Background::Background(World& world, const FVec2D& pos, const std::string& name)
	:
	Actor(world, pos, name),
	collider(std::make_shared<Collider>(*this)),
	model(GetWorld().CreateModel<TextureModel>(1, "Textures/Backgrounds/London.tga"))
{
	collider->SetVisible(true);
	collider->SetGravity(false);
	collider->SetCollisionChannel(Collider::CollisionChannel::Gravity);
	collider->SetCollisionMode(Collider::CollisionMode::Blocking);
	collider->SetRectPos(FRect(0, -500, 2000, 75));

	model->SetPosition(pos);
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

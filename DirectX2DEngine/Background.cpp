#include "Background.h"

#include "Engine.h"
#include "Graphics.h"

Background::Background(const FVec2D& pos, const std::string& name)
	:
	Actor(pos, name),
	collider(std::make_shared<Collider>(*this)),
	model(ENGINE.CreateModel<TextureModel>(1, "Textures/Backgrounds/London.tga"))
{
	collider->SetVisible(false);
	collider->SetGravity(false);
	collider->SetCollisionChannel(Collider::CollisionChannel::Gravity);
	collider->SetCollisionMode(Collider::CollisionMode::Blocking);
	collider->SetRectPos(FRect(0, -800, 2000, 200));

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

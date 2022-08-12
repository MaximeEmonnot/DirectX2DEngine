#include "Background.h"

#include "Graphics.h"

Background::Background(const FVec2D& pos, const std::string& name)
	:
	Actor(pos, name),
	collider(std::make_shared<Collider>(*this)),
	model("Textures/Backgrounds/China.tga")
{
	collider->SetCollisionChannel(Collider::CollisionChannel::Gravity);
	collider->SetCollisionMode(Collider::CollisionMode::Blocking);
	collider->SetRectPos(FRect(0, -500, 2000, 100));
	collider->SetVisible(true);
	collider->SetGravity(false);

	model.Initialize();
}

void Background::Update()
{
	Actor::Update();
	collider->Update();
}

void Background::Render()
{
	Actor::Render();
	model.Render();

	collider->Render();
}

std::vector<std::shared_ptr<Collider>> Background::GetColliders() const
{
	std::vector<std::shared_ptr<Collider>> out;
	out.emplace_back(collider);
	return out;
}

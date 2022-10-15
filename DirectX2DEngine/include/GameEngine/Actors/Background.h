#pragma once
#include "MainEngine/Collider.h"
#include "GraphicsEngine/TextureModel.h"

class Background : public Actor
{
public:
	Background(World& world, const FVec2D& pos, const std::string& name);

	virtual void Update() override;

	virtual std::vector<std::shared_ptr<Collider>> GetColliders() const override;
private:
	std::shared_ptr<Collider> collider;
	std::shared_ptr<TextureModel> model;
};

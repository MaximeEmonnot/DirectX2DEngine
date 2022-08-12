#pragma once
#include "Collider.h"
#include "TextureModel.h"

class Background : public Actor
{
public:
	Background(const FVec2D& pos, const std::string& name);

	virtual void Update() override;
	virtual void Render() override;

	virtual std::vector<std::shared_ptr<Collider>> GetColliders() const override;
private:
	std::shared_ptr<Collider> collider;
	TextureModel model;
};

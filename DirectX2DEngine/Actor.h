#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include <string>

class Actor
{
public:
	Actor() = delete;
	Actor(const DirectX::XMFLOAT2& pos, const std::string& name);
	Actor(const Actor&) = default;
	virtual ~Actor() = default;

	Actor& operator=(const Actor& toCopy);

	virtual void Update();
	virtual void Render();

	DirectX::XMFLOAT2 GetPosition() const;
	std::string GetName() const;
	//virtual std::vector<std::shared_ptr<class Collider>> GetColliders() const;

protected:
	DirectX::XMFLOAT2 pos;
	std::string name;
 };


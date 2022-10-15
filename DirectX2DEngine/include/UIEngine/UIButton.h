#pragma once

#include "AUIElement.h"

#include "GraphicsEngine/ColorModel.h"

class UIButton : public AUIElement
{
public:
	UIButton() = default;
	UIButton(const FRect& position, const DirectX::XMFLOAT4& baseColor, const DirectX::XMFLOAT4& hoverColor, const DirectX::XMFLOAT4& clickColor);

	void SetTask(std::function<void()> newTask);

	void Update() override;
	void Render() const override;

private:
	std::shared_ptr<ColorModel> pModel;
	DirectX::XMFLOAT4 baseColor;
	DirectX::XMFLOAT4 hoverColor;
	DirectX::XMFLOAT4 clickColor;

	std::function<void()> task;
};


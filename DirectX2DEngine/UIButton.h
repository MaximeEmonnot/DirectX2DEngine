#pragma once
#include "AUIElement.h"
#include "ColorModel.h"

class UIButton : public AUIElement
{
public:
	UIButton() = default;
	UIButton(const FRect& position, const DirectX::XMFLOAT4& base_color, const DirectX::XMFLOAT4& hover_color, const DirectX::XMFLOAT4& click_color);

	void SetTask(std::function<void()> new_task);

	void Update() override;
	void Render() const override;

private:
	std::shared_ptr<ColorModel> model;
	DirectX::XMFLOAT4 baseColor;
	DirectX::XMFLOAT4 hoverColor;
	DirectX::XMFLOAT4 clickColor;

	std::function<void()> task;
};


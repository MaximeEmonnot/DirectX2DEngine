#pragma once
#include <memory>

#include "AUIElement.h"
#include "GraphicsEngine/ColorModel.h"

class UIProgressBar :
    public AUIElement
{
public:
    enum class ProgressDirection
    {
	    LeftToRight,
        RightToLeft,
        TopToBottom,
        BottomToTop,
        BothHorizontal,
        BothVertical,
        AllSides
    };
public:
    UIProgressBar() = default;
    UIProgressBar(const FRect& position, const DirectX::XMFLOAT4& color, ProgressDirection direction = ProgressDirection::LeftToRight);

    void SetPercentage(float new_percentage);

    void Render() const override;

private:
    std::shared_ptr<ColorModel> backgroundModel;
    std::shared_ptr<ColorModel> fillModel;
    float percentage = 1.f;
    ProgressDirection direction;
};


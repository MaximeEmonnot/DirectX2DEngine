#pragma once

#include "AUIElement.h"

#include <memory>

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

    void    SetPercentage(float newPercentage);

    void    Render() const override;

private:
    std::shared_ptr<ColorModel>     pBackgroundModel;
    std::shared_ptr<ColorModel>     pFillModel;
    float                           percentage = 1.f;
    ProgressDirection               direction;
};


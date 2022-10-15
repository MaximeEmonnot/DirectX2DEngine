#pragma once
#include "AUICanvas.h"
#include "UIButton.h"
#include "UIImage.h"
#include "UIText.h"

class UICanvas_SinglePlayerSelection :
    public AUICanvas
{
public:
    UICanvas_SinglePlayerSelection();

    void Update() override;

private:
    std::shared_ptr<UIImage> solBadguyImage;
    std::shared_ptr<UIButton> solBadguyButton;
    std::shared_ptr<UIText> solBadguyButtonTitle;
    std::shared_ptr<UIImage> roboKyImage;
    std::shared_ptr<UIButton> roboKyButton;
    std::shared_ptr<UIText> roboKyButtonTitle;
};


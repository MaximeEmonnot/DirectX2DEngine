#pragma once
#include "UIEngine/AUICanvas.h"
#include "UIEngine/UIButton.h"
#include "UIEngine/UIImage.h"
#include "UIEngine/UIText.h"

class UICanvas_MultiPlayerSelection :
    public AUICanvas
{
public:
    UICanvas_MultiPlayerSelection();

    void Update() override;

private:
    void SelectFighter(int fighter_value) const;

private:
    std::shared_ptr<UIImage> solBadguyImage;
    std::shared_ptr<UIButton> solBadguyButton;
    std::shared_ptr<UIText> solBadguyButtonTitle;
    std::shared_ptr<UIImage> roboKyImage;
    std::shared_ptr<UIButton> roboKyButton;
    std::shared_ptr<UIText> roboKyButtonTitle;
};

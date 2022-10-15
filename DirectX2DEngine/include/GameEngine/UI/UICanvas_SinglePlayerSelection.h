#pragma once

#include "UIEngine/AUICanvas.h"
#include "UIEngine/UIButton.h"
#include "UIEngine/UIImage.h"
#include "UIEngine/UIText.h"

class UICanvas_SinglePlayerSelection :
    public AUICanvas
{
public:
    UICanvas_SinglePlayerSelection();

    void    Update() override;
private:
    void    SelectFighter(int fighterValue) const;

private:
    std::shared_ptr<UIImage>    pSolBadguyImage;
    std::shared_ptr<UIButton>   pSolBadguyButton;
    std::shared_ptr<UIText>     pSolBadguyButtonTitle;
    std::shared_ptr<UIImage>    pRoboKyImage;
    std::shared_ptr<UIButton>   pRoboKyButton;
    std::shared_ptr<UIText>     pRoboKyButtonTitle;
};


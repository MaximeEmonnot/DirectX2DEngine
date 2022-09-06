#pragma once
#include "AUICanvas.h"
#include "UIButton.h"
#include "UIImage.h"
#include "UIText.h"
#include "UIProgressBar.h"

class UICanvas_Test :
    public AUICanvas
{
public:
    UICanvas_Test();

    void Update() override;

private:
    std::shared_ptr<UIButton> button;
    std::shared_ptr<UIText> text;
};


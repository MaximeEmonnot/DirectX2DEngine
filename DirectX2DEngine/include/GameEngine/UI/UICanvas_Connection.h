#pragma once

#include "UIEngine/AUICanvas.h"
#include "UIEngine/UIButton.h"
#include "UIEngine/UIText.h"
#include "UIEngine/UITextBox.h"

class UICanvas_Connection :
    public AUICanvas
{
public:
    UICanvas_Connection();

    virtual void Update() override;
private:
    std::shared_ptr<UITextBox> pIpAddressTextBox;
    std::shared_ptr<UITextBox> pPortTextBox;
    std::shared_ptr<UIButton> pConnectionButton;
    std::shared_ptr<UIText> pConnectionButtonTitle;
};


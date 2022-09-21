#pragma once
#include "AUICanvas.h"
#include "UIButton.h"
#include "UIText.h"
#include "UITextBox.h"

class UICanvas_Connection :
    public AUICanvas
{
public:
    UICanvas_Connection();

    virtual void Update() override;
private:
    std::shared_ptr<UITextBox> ipAddressTextBox;
    std::shared_ptr<UITextBox> portTextBox;
    std::shared_ptr<UIButton> connectionButton;
    std::shared_ptr<UIText> connectionButtonTitle;
};


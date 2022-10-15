#pragma once

#include "AUIElement.h"

#include "GraphicsEngine/ColorModel.h"

class UITextBox :
    public AUIElement
{
public:
    UITextBox() = default;
    UITextBox(const FRect& position, const DirectX::XMFLOAT4& outlineColor, const std::wstring& font, const std::wstring& defaultText);

    void            SetPosition(const FRect& newPosition) override;
    void            SetOutlineColor(const DirectX::XMFLOAT4& newOutlineColor) const;

    virtual void    Update() override;
    virtual void    Render() const override;

    std::string     GetText() const;

private:
    std::shared_ptr<ColorModel>     pOutlineModel;
    std::shared_ptr<ColorModel>     pBoxModel;
    std::wstring                    font;
    std::wstring                    defaultText;
    std::wstring                    text;
    bool                            bIsFocused;
};


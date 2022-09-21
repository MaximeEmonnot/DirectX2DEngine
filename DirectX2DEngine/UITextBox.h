#pragma once
#include "ColorModel.h"

#include "AUIElement.h"
class UITextBox :
    public AUIElement
{
public:
    UITextBox() = default;
    UITextBox(const FRect& position, const DirectX::XMFLOAT4& outline_color, const std::wstring& font, const std::wstring& default_text);

    void SetPosition(const FRect& new_position) override;
    void SetOutlineColor(const DirectX::XMFLOAT4& new_outline_color) const;

    virtual void Update() override;
    virtual void Render() const override;

    std::string GetText() const;

private:
    std::shared_ptr<ColorModel> outlineModel;
    std::shared_ptr<ColorModel> boxModel;
    std::wstring font;
    std::wstring default_text;
    std::wstring text;
    bool bIsFocused;
};


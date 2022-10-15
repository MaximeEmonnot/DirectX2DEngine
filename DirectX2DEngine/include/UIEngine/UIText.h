#pragma once
#include "GraphicsEngine/TextRenderer.h"

#include "AUIElement.h"
class UIText :
    public AUIElement
{
public:
    UIText() = default;
    UIText(const FRect& position, const std::wstring& text, const std::wstring& font, float size, const D2D1_COLOR_F& color);

    void SetText(const std::wstring& new_text);
    void SetFont(const std::wstring& new_font);
    void SetSize(float new_size);
    void SetColor(const D2D1_COLOR_F& new_color);

    void Render() const override;

private:
    std::wstring text;
    std::wstring font;
    float size;
    D2D1_COLOR_F color;
};


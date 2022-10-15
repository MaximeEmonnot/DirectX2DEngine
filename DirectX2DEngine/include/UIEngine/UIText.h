#pragma once

#include "AUIElement.h"

#include "GraphicsEngine/TextRenderer.h"

class UIText :
    public AUIElement
{
public:
    UIText() = default;
    UIText(const FRect& position, const std::wstring& text, const std::wstring& font, float size, const D2D1_COLOR_F& color);

    void    SetText(const std::wstring& newText);
    void    SetFont(const std::wstring& newFont);
    void    SetSize(float newSize);
    void    SetColor(const D2D1_COLOR_F& newColor);
            
    void    Render() const override;

private:
    std::wstring    text;
    std::wstring    font;
    float           size;
    D2D1_COLOR_F    color;
};


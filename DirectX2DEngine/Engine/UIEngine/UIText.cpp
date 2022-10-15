#include "UIEngine/UIText.h"

UIText::UIText(const FRect& position, const std::wstring& text, const std::wstring& font, float size, const D2D1_COLOR_F& color)
	:
	AUIElement(position),
	text(text),
	font(font),
	size(size),
	color(color)
{
}

void UIText::SetText(const std::wstring& newText)
{
	text = newText;
}

void UIText::SetFont(const std::wstring& newFont)
{
	font = newFont;
}

void UIText::SetSize(float newSize)
{
	size = newSize;
}

void UIText::SetColor(const D2D1_COLOR_F& newColor)
{
	color = newColor;
}

void UIText::Render() const
{
	TEXT_ENGINE.Render(text, font, size, position, color);
}

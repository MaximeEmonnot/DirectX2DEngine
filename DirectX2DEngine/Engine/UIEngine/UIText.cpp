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

void UIText::SetText(const std::wstring& new_text)
{
	text = new_text;
}

void UIText::SetFont(const std::wstring& new_font)
{
	font = new_font;
}

void UIText::SetSize(float new_size)
{
	size = new_size;
}

void UIText::SetColor(const D2D1_COLOR_F& new_color)
{
	color = new_color;
}

void UIText::Render() const
{
	TEXT_ENGINE.Render(text, font, size, position, color);
}

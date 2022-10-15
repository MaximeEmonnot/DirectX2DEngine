#include "UIEngine/UITextBox.h"

#include <codecvt>
#include <locale>

#include "CoreEngine/Keyboard.h"
#include "CoreEngine/Mouse.h"
#include "GraphicsEngine/TextRenderer.h"

UITextBox::UITextBox(const FRect& position, const DirectX::XMFLOAT4& outline_color, const std::wstring& font, const std::wstring& default_text)
	:
	AUIElement(position),
	outlineModel(std::make_shared<ColorModel>(FRect(position.pos, position.width + 5, position.height + 5), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
	boxModel(std::make_shared<ColorModel>(position, DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f))),
	font(font),
	default_text(default_text),
	bIsFocused(false)
{
	outlineModel->Initialize();
	outlineModel->SetDepth(depth - 2.f);

	boxModel->Initialize();
	boxModel->SetDepth(depth);
}

void UITextBox::SetPosition(const FRect& new_position)
{
	AUIElement::SetPosition(new_position);
	outlineModel->SetRectangle(FRect(new_position.pos, new_position.width, new_position.height));
	boxModel->SetRectangle(new_position);
}

void UITextBox::SetOutlineColor(const DirectX::XMFLOAT4& new_outline_color) const
{
	outlineModel->SetColor(new_outline_color);
}

void UITextBox::Update()
{
	if (MOUSE.Read() == Mouse::EventType::LPress)
		bIsFocused = position.ContainsVec2D(MOUSE_POSITION);

	if (bIsFocused)
	{
		const char c = KBD.ReadChar();
		if (c >= 32) text += c;
		else if (c == 8 && !text.empty()) text.erase(text.size() - 1);
	}
}

void UITextBox::Render() const
{
	boxModel->Render();
	outlineModel->Render();

	if (bIsFocused || !text.empty()) TEXT_ENGINE.Render(text, font, position.height / 2.f, position, D2D1::ColorF(D2D1::ColorF::White));
	else TEXT_ENGINE.Render(default_text, font, position.height / 2.f, position, D2D1::ColorF(D2D1::ColorF::LightGray));
}

std::string UITextBox::GetText() const
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
	return convert.to_bytes(text);
}

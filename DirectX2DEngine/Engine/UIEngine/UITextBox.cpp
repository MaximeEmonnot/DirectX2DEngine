#include "UIEngine/UITextBox.h"

#include <codecvt>
#include <locale>

#include "CoreEngine/Keyboard.h"
#include "CoreEngine/Mouse.h"
#include "GraphicsEngine/TextRenderer.h"

UITextBox::UITextBox(const FRect& position, const DirectX::XMFLOAT4& outlineColor, const std::wstring& font, const std::wstring& defaultText)
	:
	AUIElement(position),
	pOutlineModel(std::make_shared<ColorModel>(FRect(position.pos, position.width + 5, position.height + 5), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
	pBoxModel(std::make_shared<ColorModel>(position, DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f))),
	font(font),
	defaultText(defaultText),
	bIsFocused(false)
{
	pOutlineModel->Initialize();
	pOutlineModel->SetDepth(depth - 2.f);

	pBoxModel->Initialize();
	pBoxModel->SetDepth(depth);
}

void UITextBox::SetPosition(const FRect& newPosition)
{
	AUIElement::SetPosition(newPosition);
	pOutlineModel->SetRectangle(FRect(newPosition.pos, newPosition.width, newPosition.height));
	pBoxModel->SetRectangle(newPosition);
}

void UITextBox::SetOutlineColor(const DirectX::XMFLOAT4& newOutlineColor) const
{
	pOutlineModel->SetColor(newOutlineColor);
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
	pBoxModel->Render();
	pOutlineModel->Render();

	if (bIsFocused || !text.empty()) TEXT_ENGINE.Render(text, font, position.height / 2.f, position, D2D1::ColorF(D2D1::ColorF::White));
	else TEXT_ENGINE.Render(defaultText, font, position.height / 2.f, position, D2D1::ColorF(D2D1::ColorF::LightGray));
}

std::string UITextBox::GetText() const
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
	return convert.to_bytes(text);
}

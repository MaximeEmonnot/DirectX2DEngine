#pragma once

#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#include <dwrite.h>
#include <d2d1.h>
#include <wrl.h>
#include <string>
#include <map>

#include "MathEngine/Rect.h"

#define TEXT_ENGINE TextRenderer::GetInstance()

class TextRenderer
{
public:
	TextRenderer();
	TextRenderer(const TextRenderer&) = delete;
	TextRenderer& operator=(const TextRenderer&) = delete;
	~TextRenderer() = default;

	static TextRenderer&	GetInstance();

	void					Render(const std::wstring& text, const std::wstring& fontFamily, float fontSize, 
								   FRect position, D2D1_COLOR_F fontColor = D2D1::ColorF(D2D1::ColorF::White));

public:
	static std::unique_ptr<TextRenderer>													pInstance;

	Microsoft::WRL::ComPtr<IDWriteFactory>													pDWriteFactory;
	Microsoft::WRL::ComPtr<ID2D1Factory>													pD2DFactory;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget>												pRenderTarget;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>											pTextBrush;

	std::map<std::pair<std::wstring, float>, Microsoft::WRL::ComPtr<IDWriteTextFormat>>		fonts;
};


#include "GraphicsEngine/TextRenderer.h"

#include "CoreEngine/Window.h"
#include "GraphicsEngine/Graphics.h"

// STATIC VARIABLES INITILIZATION

std::unique_ptr<TextRenderer> TextRenderer::pInstance = nullptr;

//******//

TextRenderer::TextRenderer()
{
	HRESULT hr = S_OK;

	// D2D1 Factory Creation

	if (FAILED(hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		pD2DFactory.GetAddressOf()
	)))
		throw GFX_EXCEPTION("An exception has been caught during D2D Factory Creation.", hr);

	// DWrite Factory Creation

	if (FAILED(hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(pDWriteFactory.GetAddressOf())
	)))
		throw GFX_EXCEPTION("An exception has been caught during DWrite Factory Creation.", hr);

	// Direct3D Back Buffer Retrieval

	Microsoft::WRL::ComPtr<IDXGISurface> pBackBuffer;
	if (FAILED(hr = GFX.GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(pBackBuffer.GetAddressOf()))))
		throw GFX_EXCEPTION("An exception has been caught during Text Surface Buffer Creation.", hr);

	const float dpi = static_cast<float>(GetDpiForWindow(WND.GetHWND()));

	const D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpi,
			dpi
		);

	// Text Surface Render Creation (using Direct3D Back Buffer)

	if (FAILED(hr = pD2DFactory->CreateDxgiSurfaceRenderTarget(pBackBuffer.Get(), &props, pRenderTarget.GetAddressOf())))
		throw GFX_EXCEPTION("An exception has been caught during DXGI Surface Render Target Creation.", hr);

	// Solid Color Brush Creation

	if (FAILED(hr = pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		pTextBrush.GetAddressOf()
	)))
		throw GFX_EXCEPTION("An exception has been caught during Text Brush Creation.", hr);
}

TextRenderer& TextRenderer::GetInstance()
{
	if (!pInstance) 
		pInstance = std::make_unique<TextRenderer>();
	return *pInstance;
}

void TextRenderer::Render(const std::wstring& text, const std::wstring& fontFamily, float fontSize, FRect position, D2D1_COLOR_F fontColor)
{
	HRESULT hr = S_OK;

	// First we check if the font has been initialized

	if (!fonts.contains(std::pair(fontFamily, fontSize))) {

		// If not, we create a text format using the font informations
		Microsoft::WRL::ComPtr<IDWriteTextFormat> text_format;
		if (FAILED(hr = pDWriteFactory->CreateTextFormat(
			fontFamily.c_str(),
			nullptr,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			fontSize,
			L"en-us",
			text_format.GetAddressOf()
		)))
			throw GFX_EXCEPTION("An exception has been caught during Text Format Creation.", hr);

		// Base Text and paragraph alignments
		if (FAILED(hr = text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER)))
			throw GFX_EXCEPTION("An exception has been caught during Text Format Text Alignment.", hr);

		if (FAILED(hr = text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER)))
			throw GFX_EXCEPTION("An exception has been caught during Text Format Paragraph Alignment.", hr);

		// We add our new font to the map as we want to use it later
		fonts.emplace(std::pair(std::pair(fontFamily, fontSize), text_format));
	}

	// We set the Solid Brush Color, as we will use this to draw text
	pTextBrush->SetColor(fontColor);

	// We convert our coordinates from Mathematical Space to Graphic Space 
	const D2D1_RECT_F layout_rect = D2D1::RectF(
		position.pos.x + (WND.GetWidth() - position.width) / 2,
		-position.pos.y + (WND.GetHeight() - position.height) / 2,
		position.pos.x + (WND.GetWidth() + position.width) / 2,
		-position.pos.y + (WND.GetHeight() + position.height) / 2
	);

	// Finally, we draw our text

	pRenderTarget->BeginDraw();
	pRenderTarget->SetTransform(D2D1::IdentityMatrix());
	pRenderTarget->DrawText(
		text.c_str(),
		static_cast<UINT32>(text.size()),
		fonts.at(std::pair(fontFamily, fontSize)).Get(),
		layout_rect,
		pTextBrush.Get()
	);
	pRenderTarget->EndDraw();
}

#include "Graphics.h"

#include "Window.h"

// STATIC VARIABLES INITIALIZATION
std::unique_ptr<Graphics> Graphics::pInstance = nullptr;

Graphics::Graphics()
{
	HRESULT hr = S_OK;

	// Create Device and Swap Chain

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;
	scd.BufferDesc.Width = WND.width;
	scd.BufferDesc.Height = WND.height;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 1;
	scd.BufferDesc.RefreshRate.Denominator = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = WND.hWnd;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = TRUE;

	UINT flags = 0u;
#if _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	if (FAILED(hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		flags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pDeviceContext)))
		throw GFX_EXCEPTION("An exception has been caught during Device and Swap Chain Creation.", hr);

	// Create Back Buffer
	if (FAILED(hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer)))
		throw GFX_EXCEPTION("An exception has been caught during Back Buffer Creation.", hr);

	// Create Render Target
	if (FAILED(hr = pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, pRenderTarget.GetAddressOf())))
		throw GFX_EXCEPTION("An exception has been caught during Render Target Creation.", hr);

	// Create Depth Stencil Buffer
	D3D11_TEXTURE2D_DESC ds_data;
	ZeroMemory(&ds_data, sizeof(D3D11_TEXTURE2D_DESC));
	ds_data.Width = static_cast<INT>(WND.width);
	ds_data.Height = static_cast<INT>(WND.height);
	ds_data.MipLevels = 1;
	ds_data.ArraySize = 1;
	ds_data.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	ds_data.SampleDesc.Count = 1;
	ds_data.SampleDesc.Quality = 0;
	ds_data.Usage = D3D11_USAGE_DEFAULT;
	ds_data.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ds_data.CPUAccessFlags = 0;
	ds_data.MiscFlags = 0;

	if (FAILED(hr = pDevice->CreateTexture2D(&ds_data, nullptr, pDepthStencilBuffer.GetAddressOf())))
		throw GFX_EXCEPTION("An exception has been caught during Depth Stencil Buffer Creation.", hr);

	// Create and Set Depth-Stencil State
	D3D11_DEPTH_STENCIL_DESC ds_desc;
	ZeroMemory(&ds_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	ds_desc.DepthEnable = TRUE;
	ds_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds_desc.DepthFunc = D3D11_COMPARISON_LESS;
	ds_desc.StencilEnable = TRUE;
	ds_desc.StencilReadMask = 0xFF;
	ds_desc.StencilWriteMask = 0xFF;
	ds_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	ds_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	ds_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	ds_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	ds_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	ds_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	ds_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	ds_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	if (FAILED(hr = pDevice->CreateDepthStencilState(&ds_desc, pDepthStencilState.GetAddressOf())))
		throw GFX_EXCEPTION("An exception has been caught during Depth Stencil State Creation.", hr);

	pDeviceContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1);

	// Create and Set Depth-Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
	ZeroMemory(&dsv_desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv_desc.Texture2D.MipSlice = 0;

	if (FAILED(hr = pDevice->CreateDepthStencilView(pDepthStencilBuffer.Get(), &dsv_desc, pDepthStencilView.GetAddressOf())))
		throw GFX_EXCEPTION("An exception has been caught during Depth Stencil View Creation.", hr);

	// Set Render Target with Depth Stencil View

	pDeviceContext->OMSetRenderTargets(1, pRenderTarget.GetAddressOf(), pDepthStencilView.Get());

	// Create and Set Blend State (Alpha Blending)

	D3D11_BLEND_DESC bs_desc;
	ZeroMemory(&bs_desc, sizeof(D3D11_BLEND_DESC));

	bs_desc.RenderTarget[0].BlendEnable = TRUE;

	bs_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bs_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bs_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	bs_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bs_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bs_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bs_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(hr = pDevice->CreateBlendState(&bs_desc, pBlendState.GetAddressOf())))
		throw GFX_EXCEPTION("An exception has been caught during Blend State Creation.", hr);

	pDeviceContext->OMSetBlendState(pBlendState.Get(), nullptr, 0xffffff);

	// Create and Set Viewport
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = static_cast<FLOAT>(WND.width);
	vp.Height = static_cast<FLOAT>(WND.height);
	vp.MaxDepth = 1.f;
	vp.MinDepth = 0.f;

	pDeviceContext->RSSetViewports(1, &vp);
}

Graphics& Graphics::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<Graphics>();
	return *pInstance;
}

void Graphics::BeginDraw() const
{
	// Clear Render Target
	constexpr float color[4] = { 0.f, 0.f, 0.f, 0.f };
	pDeviceContext->ClearRenderTargetView(pRenderTarget.Get(), color);
	pDeviceContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
}

void Graphics::EndDraw() const
{
	pSwapChain->Present(0, 0);
}

ID3D11Device* Graphics::GetDevice() const
{
	return pDevice.Get();
}

ID3D11DeviceContext* Graphics::GetDeviceContext() const
{
	return pDeviceContext.Get();
}

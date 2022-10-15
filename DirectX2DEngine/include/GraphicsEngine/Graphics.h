#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "DirectX.h"

#include "DebugEngine/EngineException.h"

#define GFX Graphics::GetInstance()
#define GFX_EXCEPTION(note, hr) Graphics::GraphicsException("DirectX 2D Engine - Graphics Engine Exception", note, __FILE__, __LINE__, hr)

class Graphics
{
public:
	class GraphicsException : public EngineException
	{
	public:
		GraphicsException(const std::string& type, const std::string& note, const std::string& file, unsigned int line, HRESULT hr) noexcept
			:
			EngineException(type, note, file, line)
		{
			std::stringstream ss;
			ss << std::hex << hr;
			infos += "\nError code : " + ss.str();
		}
	};

public:
	Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator= (const Graphics&) = delete;

	static Graphics& GetInstance();

	void BeginDraw() const;
	void EndDraw() const;

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;
	IDXGISwapChain* GetSwapChain() const;

private:
	static std::unique_ptr<Graphics> pInstance;

	// Direct3D Engine
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTarget;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11BlendState> pBlendState;
};


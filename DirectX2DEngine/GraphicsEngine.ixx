module;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#include <dwrite.h>
#include <d2d1.h>
#include <wrl.h>

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>

namespace Shaders
{
#include "Shaders/ColorVertexShader.shh"
#include "Shaders/ColorPixelShader.shh"
#include "Shaders/TextureVertexShader.shh"
#include "Shaders/TexturePixelShader.shh"
}

export module GraphicsEngine;

import <string>;
import <sstream>;
import <map>;
import <vector>;
import <functional>;

import MathEngine;
import DebugEngine;
import CoreEngine;
import ParserEngine;

export class GraphicsException : public EngineException
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

export GraphicsException GFX_EXCEPTION(const std::string& note, HRESULT hr)
{
	return GraphicsException("DirectX 2D Engine - Graphics Engine Exception", note, __FILE__, __LINE__, hr);
}

export
class Graphics
{
public:
	Graphics()
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

		UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
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
	Graphics(const Graphics&) = delete;
	Graphics& operator= (const Graphics&) = delete;

	static Graphics& GetInstance()
	{
		if (!pInstance)
			pInstance = std::make_unique<Graphics>();
		return *pInstance;
	}

	void					BeginDraw() const
	{
		// Clear Render Target
		constexpr float color[4] = { 0.f, 0.f, 0.f, 1.f };
		pDeviceContext->ClearRenderTargetView(pRenderTarget.Get(), color);
		pDeviceContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
	}
	void					EndDraw() const
	{
		pSwapChain->Present(0, 0);
	}

	ID3D11Device* GetDevice() const
	{
		return pDevice.Get();
	}
	ID3D11DeviceContext* GetDeviceContext() const
	{
		return pDeviceContext.Get();
	}
	IDXGISwapChain* GetSwapChain() const
	{
		return pSwapChain.Get();
	}

private:
	static std::unique_ptr<Graphics>					pInstance;

	// Direct3D Engine
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTarget;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pDepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		pDepthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11BlendState>			pBlendState;
};
// STATIC VARIABLES INITIALIZATION
std::unique_ptr<Graphics> Graphics::pInstance = nullptr;

export Graphics& GFX = Graphics::GetInstance();


export
class Texture
{
private:
	struct TGAHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};
public:
	Texture() = default;
	Texture(const std::string& filepath)
	{
		HRESULT hr = S_OK;

		unsigned char* tga_data = LoadTGA(filepath);

		// Texture2D Creation 
		D3D11_TEXTURE2D_DESC tex_desc;
		ZeroMemory(&tex_desc, sizeof(D3D11_TEXTURE2D_DESC));

		tex_desc.Width = width;
		tex_desc.Height = height;
		tex_desc.MipLevels = 0;
		tex_desc.ArraySize = 1;
		tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.SampleDesc.Count = 1;
		tex_desc.SampleDesc.Quality = 0;
		tex_desc.Usage = D3D11_USAGE_DEFAULT;
		tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		tex_desc.CPUAccessFlags = 0;
		tex_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		if (FAILED(hr = GFX.GetDevice()->CreateTexture2D(&tex_desc, nullptr, pTexture.GetAddressOf())))
			throw GFX_EXCEPTION("An exception has been caught during Texture Object Creation.", hr);

		// Linking the TGA data to the Texture2D Object
		const int row_pitch = (width * 4) * sizeof(unsigned char);
		GFX.GetDeviceContext()->UpdateSubresource(pTexture.Get(), 0, nullptr, tga_data, row_pitch, 0);

		// Shader Resource View Creation using the Texture2D data
		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
		ZeroMemory(&srv_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srv_desc.Format = tex_desc.Format;
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MostDetailedMip = 0;
		srv_desc.Texture2D.MipLevels = -1;

		if (FAILED(hr = GFX.GetDevice()->CreateShaderResourceView(pTexture.Get(), &srv_desc, pTextureView.GetAddressOf())))
			throw GFX_EXCEPTION("An exception has been caught during Texture Object Shader Resource View Creation.", hr);

		GFX.GetDeviceContext()->GenerateMips(pTextureView.Get());

		delete[] tga_data;
		tga_data = nullptr;
	}
	Texture(const Texture& tex)
		:
		pTexture(tex.pTexture),
		pTextureView(tex.pTextureView),
		width(tex.width),
		height(tex.height)
	{}

	virtual ~Texture() = default;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	GetShaderResourceView() const
	{
		return pTextureView;
	}

	int													GetWidth() const
	{
		return width;
	}
	int													GetHeight() const
	{
		return height;
	}

protected:
	unsigned char* LoadTGA(const std::string& filepath)
	{
		FILE* pFile;
		TGAHeader tgaFileHeader = {};
		fopen_s(&pFile, filepath.c_str(), "rb");

		// First we read the TGA header...
		unsigned int count = static_cast<unsigned int>(fread(&tgaFileHeader, sizeof(TGAHeader), 1, pFile));

		width = static_cast<int>(tgaFileHeader.width);
		height = static_cast<int>(tgaFileHeader.height);

		//...we also make sure that a single pixel is made of 32 bits (RGBA)
		if (tgaFileHeader.bpp != 32)
			throw GFX_EXCEPTION("An exception has occured during TGA file reading. Please use TGA 32 bits.", S_FALSE);

		// Then we read the entire TGA image...
		const int imageSize = width * height * 4;
		unsigned char* tgaImage = new unsigned char[imageSize];
		fread(tgaImage, 1, imageSize, pFile);
		fclose(pFile);
		unsigned char* tga_data = new unsigned char[imageSize];
		int index = 0;
		int k = (width * height * 4) - (width * 4);

		// ...and we store it while making sure to invert the rows
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				tga_data[index + 0] = tgaImage[k + 2];
				tga_data[index + 1] = tgaImage[k + 1];
				tga_data[index + 2] = tgaImage[k + 0];
				tga_data[index + 3] = tgaImage[k + 3];

				k += 4;
				index += 4;
			}
			k -= (width * 8);
		}

		delete[] tgaImage;
		tgaImage = nullptr;

		return tga_data;
	}

protected:
	int													width = 0;
	int													height = 0;

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pTextureView;
};


export
class AnimationTexture : public Texture
{
public:
	AnimationTexture(const std::string& filePath)
		:
		Texture(filePath)
	{
		const unsigned char* tga_data = LoadTGA(filePath);

		// Reading Center Data from the first pixel (This pixel won't be drawn because its alpha value is null)
		center = IVec2D(tga_data[0] << 4 | tga_data[1] >> 4,
			(tga_data[1] & 15) << 8 | tga_data[2]);
		center.x -= width / 2;
		center.y = -center.y + height / 2;

		delete[] tga_data;
	}

	virtual ~AnimationTexture() override = default;

	Texture& operator()()
	{
		return *dynamic_cast<Texture*>(this);
	}

	IVec2D	GetCenter() const
	{
		return center;
	}

private:
	IVec2D	center;
};


export
class TextureFactory
{
public:
	TextureFactory() = default;
	TextureFactory(const TextureFactory&) = delete;
	TextureFactory& operator=(const TextureFactory&) = delete;

	static TextureFactory& GetInstance()
	{
		if (!pInstance)
			pInstance = std::make_unique<TextureFactory>();
		return *pInstance;
	}

	Texture& GetTexture(const std::string& path)
	{
		// We check whether an initialization is needed or not
		if (!textureMap.contains(path))
			textureMap.emplace(path, std::make_shared<Texture>(path));
		return *textureMap.at(path);
	}
	AnimationTexture& GetAnimationTexture(const std::string& path)
	{
		// We check whether an initialization is needed or not
		if (!textureMap.contains(path))
			textureMap.emplace(path, std::make_shared<AnimationTexture>(path));
		const std::shared_ptr<AnimationTexture> out = std::dynamic_pointer_cast<AnimationTexture>(textureMap.at(path));
		return *out;
	}

private:
	static std::unique_ptr<TextureFactory>				pInstance;

	std::map<std::string, std::shared_ptr<Texture>>		textureMap;
};
// STATIC VARIABLES INITIALIZATION
std::unique_ptr<TextureFactory> TextureFactory::pInstance = nullptr;

export Texture& TEXTURE(const std::string& path)
{
	return TextureFactory::GetInstance().GetTexture(path);
}
export AnimationTexture& ANIMATION_TEXTURE(const std::string& path)
{
	return TextureFactory::GetInstance().GetAnimationTexture(path);
}


export
class Animation {
public:
	enum class AnimationMode
	{
		NoLoop,
		LoopingAll,
		LoopingBack
	};
public:
	Animation(std::vector<std::string> frames, float holdTime, AnimationMode animMode)
		:
		frames(frames),
		animMode(animMode),
		holdTime(holdTime)
	{}

	void				Update()
	{
		curFrameTime += DELTATIME;
		while (curFrameTime > holdTime) Advance();
	}

	AnimationTexture& GetTexture() const
	{
		return ANIMATION_TEXTURE(frames.at(iCurFrame));
	}

	void				Reset()
	{
		iCurFrame = 0;
		threshold = 1;
		bIsFinished = false;
	}
	bool				IsFinished() const
	{
		return bIsFinished;
	}
private:
	void				Advance()
	{
		// We go into the next frame, depending on the AnimationLoopingMode
		iCurFrame += threshold;
		if (iCurFrame >= frames.size() || iCurFrame < 0) {
			switch (animMode)
			{
			case AnimationMode::NoLoop:
				iCurFrame = static_cast<int>(frames.size() - 1);
				bIsFinished = true;
				break;
			case AnimationMode::LoopingAll:
				iCurFrame = 0;
				break;
			case AnimationMode::LoopingBack:
				bIsFinished = (iCurFrame < 0);
				threshold *= -1;
				iCurFrame += 2 * threshold;
				break;
			default:
				break;
			}
		}
		else bIsFinished = false;
		curFrameTime -= holdTime;
	}
private:
	std::vector<std::string>	frames;

	const std::string			spriteName;
	const AnimationMode			animMode;
	float						curFrameTime = 0.0f;
	float						holdTime;
	int							iCurFrame = 0;
	int							threshold = 1;
	bool						bIsFinished = false;
};


export
class AnimationSystem
{
public:
	AnimationSystem(const std::string& filePath)
	{
		JSONParser::Reader jsonReader = JSON(filePath);
		const std::string character = jsonReader.GetValueOf("character").GetString();
		auto& v = jsonReader.GetValueOf("animations");
		for (auto itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr)
		{
			const std::string anim_name = itr->name.GetString();
			std::vector<std::string> sprites;
			const int count = itr->value.GetArray()[0].GetInt();
			const float holdTime = itr->value.GetArray()[1].GetFloat();
			const Animation::AnimationMode animation_mode = static_cast<Animation::AnimationMode>(itr->value.GetArray()[2].GetInt());

			for (int i = 0; i < count; i++) sprites.push_back(character + anim_name + std::string("\\") + std::to_string(i) + std::string(".tga"));

			animations.insert(std::pair(anim_name, Animation(sprites, holdTime, animation_mode)));

			if (animState.empty()) animState = anim_name;
		}
	}

	void						Update()
	{
		animations.at(animState).Update();
		for (auto& entry : transitions) if (entry.first == animState && entry.second.second()) SetAnimation(entry.second.first);
	}

	void						SetAnimation(const std::string& animName)
	{
		if (animState != animName) {
			animState = animName;
			animations.at(animState).Reset();
		}
	}
	void						AddTransition(const std::string& from, const std::string& to, const std::function<bool()>& condition)
	{
		transitions.insert(std::pair(from, std::pair(to, condition)));
	}

	AnimationTexture& GetTexture() const
	{
		return animations.at(animState).GetTexture();
	}

	std::string					GetCurrentAnimation() const
	{
		return animState;
	}

	std::vector<std::string>	GetAnimationList() const
	{
		std::vector<std::string> out;
		for (auto& entry : animations) out.emplace_back(entry.first);
		return out;
	}

	bool						AnimationIsFinished() const
	{
		return animations.at(animState).IsFinished();
	}
private:
	std::string																				animState = "";
	std::unordered_map<std::string, Animation>												animations;
	std::unordered_multimap<std::string, std::pair<std::string, std::function<bool()>>>		transitions;
};


class BaseModel;
class BaseShader;

export
class BaseShader
{
public:
	BaseShader() = default;
	virtual ~BaseShader() = default;

	virtual void			Initialize()
	{
		HRESULT hr = S_OK;
		if (FAILED(hr = InputLayoutCreation()))
			throw GFX_EXCEPTION("An error has been caught during Shader Object Input Layout Creation.", hr);
		if (FAILED(hr = VertexShaderCreation()))
			throw GFX_EXCEPTION("An error has been caught during Shader Object Vertex Shader Creation.", hr);
		if (FAILED(hr = PixelShaderCreation()))
			throw GFX_EXCEPTION("An error has been caught during Shader Object Pixel Shader Creation.", hr);
		if (FAILED(hr = ConstantBufferCreation()))
			throw GFX_EXCEPTION("An error has been caught during Shader Object Constant Buffer Creation.", hr);

	}
	virtual void			RenderModel(BaseModel& model) = 0;

	ID3D11InputLayout* GetInputLayout() const
	{
		return pInputLayout.Get();
	}
	ID3D11VertexShader* GetVertexShader() const
	{
		return pVertexShader.Get();
	}
	ID3D11PixelShader* GetPixelShader() const
	{
		return pPixelShader.Get();
	}

protected:
	virtual HRESULT			InputLayoutCreation() = 0;
	virtual HRESULT			VertexShaderCreation() = 0;
	virtual HRESULT			PixelShaderCreation() = 0;
	virtual HRESULT			ConstantBufferCreation() = 0;

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		pConstantBuffer;
};

export
class BaseModel
{
public:
	BaseModel() = default;
	virtual ~BaseModel() = default;

	void			Initialize()
	{
		SettingShader();
		SettingBufferConstants();
		SettingRenderingConstants();
		SettingVertices();
		SettingIndices();

		HRESULT hr = S_OK;

		if (FAILED(hr = SettingVertexBuffers()))
			throw GFX_EXCEPTION("An exception has been caught during Model Vertex Buffer Initialization.", hr);

		if (FAILED(hr = SettingIndexBuffer()))
			throw GFX_EXCEPTION("An exception has been caught during Model Index Buffer Initialization.", hr);

	}

	void			Render()
	{
		// Render Model
		if (bIsInverted) GFX.GetDeviceContext()->IASetVertexBuffers(0, 1, pInvertedVertexBuffer.GetAddressOf(), &stride, &offset);
		else GFX.GetDeviceContext()->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);
		GFX.GetDeviceContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		GFX.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Render Shader
		pShader->RenderModel(*this);
	}
	int				GetVertices() const
	{
		return nVertices;
	}

	void			SetDepth(float newDepth)
	{
		depth = newDepth;
	}
	float			GetDepth() const
	{
		return (512.f - depth) / 512.f;
	}

	void			SetVisibility(bool bValue)
	{
		bIsVisible = bValue;
	}
	bool			IsVisible() const
	{
		return bIsVisible;
	}

	void			SetInverted(bool bValue)
	{
		bIsInverted = bValue;
	}
	bool			IsInverted() const
	{
		return bIsInverted;
	}

protected:
	virtual void	SettingShader() = 0;
	virtual void	SettingBufferConstants() = 0;
	virtual void	SettingRenderingConstants() = 0;
	virtual void	SettingVertices() = 0;

	void			SettingIndices()
	{
		// As we only use planes, the IndexBuffer is quite simple
		pIndices = new unsigned long[nVertices];
		for (int i = 0; i < nVertices; i++)
			pIndices[i] = i;
	}
private:
	HRESULT			SettingVertexBuffers()
	{
		HRESULT hr = S_OK;

		// Vertex Buffer Creation
		D3D11_BUFFER_DESC vb_desc;
		ZeroMemory(&vb_desc, sizeof(D3D11_BUFFER_DESC));
		vb_desc.Usage = D3D11_USAGE_DEFAULT;
		vb_desc.ByteWidth = vertexBufferByteWidth;
		vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vb_data;
		ZeroMemory(&vb_data, sizeof(D3D11_SUBRESOURCE_DATA));
		vb_data.pSysMem = pVertices;

		hr = GFX.GetDevice()->CreateBuffer(&vb_desc, &vb_data, &pVertexBuffer);

		// Inverted Vertex Buffer Creation (used for flipped models)
		D3D11_BUFFER_DESC ivb_desc;
		ZeroMemory(&ivb_desc, sizeof(D3D11_BUFFER_DESC));
		ivb_desc.Usage = D3D11_USAGE_DEFAULT;
		ivb_desc.ByteWidth = vertexBufferByteWidth;
		ivb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA ivb_data;
		ZeroMemory(&ivb_data, sizeof(D3D11_SUBRESOURCE_DATA));
		ivb_data.pSysMem = pInvertedVertices;

		hr = GFX.GetDevice()->CreateBuffer(&ivb_desc, &ivb_data, &pInvertedVertexBuffer);

		return hr;
	}
	HRESULT			SettingIndexBuffer()
	{
		D3D11_BUFFER_DESC ib_desc;
		ZeroMemory(&ib_desc, sizeof(D3D11_BUFFER_DESC));
		ib_desc.Usage = D3D11_USAGE_DEFAULT;
		ib_desc.ByteWidth = sizeof(unsigned long) * nVertices;
		ib_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA ib_data;
		ZeroMemory(&ib_data, sizeof(D3D11_SUBRESOURCE_DATA));
		ib_data.pSysMem = pIndices;

		return GFX.GetDevice()->CreateBuffer(&ib_desc, &ib_data, &pIndexBuffer);
	}

protected:
	std::shared_ptr<BaseShader>				pShader;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	pInvertedVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	pIndexBuffer;

	bool									bIsVisible = true;
	bool									bIsInverted = false;

	unsigned int							vertexBufferByteWidth = 0u;
	int										nVertices = 0;
	unsigned int							stride = 0u;
	unsigned int							offset = 0u;

	void* pVertices = nullptr;
	void* pInvertedVertices = nullptr;
	unsigned long* pIndices = nullptr;

	float									depth = 0.f;
};


export
class ColorShader :
	public BaseShader
{
public:
	struct VertexInput
	{
		DirectX::XMFLOAT3   position;
	};

	struct VariableBuffer
	{
		DirectX::XMFLOAT2   offset;
		DirectX::XMMATRIX   rotation;
		DirectX::XMMATRIX   scale;
		DirectX::XMFLOAT4   color;
		float               depth;
	};

public:
	ColorShader() = default;

	virtual void        RenderModel(BaseModel& model) override
	{
		HRESULT hr = S_OK;
		// Rendering Model
		GFX.GetDeviceContext()->IASetInputLayout(pInputLayout.Get());
		GFX.GetDeviceContext()->VSSetShader(pVertexShader.Get(), nullptr, 0);

		// We pass all the data needed for the Shader's Constant Buffer
		D3D11_MAPPED_SUBRESOURCE mapped_subresource;
		if (FAILED(hr = GFX.GetDeviceContext()->Map(pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource)))
			throw GFX_EXCEPTION("An exception has been caught during Shader Object Subresource Mapping.", hr);

		VariableBuffer* data = static_cast<VariableBuffer*>(mapped_subresource.pData);
		data->offset = dynamic_cast<ColorModel*>(&model)->GetOffset();
		data->rotation = dynamic_cast<ColorModel*>(&model)->GetRotationMatrix();
		data->scale = dynamic_cast<ColorModel*>(&model)->GetScaleMatrix();
		data->color = dynamic_cast<ColorModel*>(&model)->GetColor();
		data->depth = model.GetDepth();

		GFX.GetDeviceContext()->Unmap(pConstantBuffer.Get(), 0);
		GFX.GetDeviceContext()->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());

		// Finishing rendering Model

		GFX.GetDeviceContext()->PSSetShader(pPixelShader.Get(), nullptr, 0);
		GFX.GetDeviceContext()->Draw(model.GetVertices(), 0);
	}

protected:
	virtual HRESULT     InputLayoutCreation() override
	{
		constexpr D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		return GFX.GetDevice()->CreateInputLayout(ied, 1, Shaders::ColorVertexShaderBytecode, sizeof(Shaders::ColorVertexShaderBytecode), pInputLayout.GetAddressOf());
	}
	virtual HRESULT     VertexShaderCreation() override
	{
		return GFX.GetDevice()->CreateVertexShader(Shaders::ColorVertexShaderBytecode, sizeof(Shaders::ColorVertexShaderBytecode), nullptr, pVertexShader.GetAddressOf());
	}
	virtual HRESULT     PixelShaderCreation() override
	{
		return GFX.GetDevice()->CreatePixelShader(Shaders::ColorPixelShaderBytecode, sizeof(Shaders::ColorPixelShaderBytecode), nullptr, pPixelShader.GetAddressOf());
	}
	virtual HRESULT     ConstantBufferCreation() override
	{
		D3D11_BUFFER_DESC cb_desc;
		ZeroMemory(&cb_desc, sizeof(D3D11_BUFFER_DESC));
		cb_desc.Usage = D3D11_USAGE_DYNAMIC;
		cb_desc.ByteWidth = sizeof(VariableBuffer);
		cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb_desc.MiscFlags = 0;
		cb_desc.StructureByteStride = 0;

		return GFX.GetDevice()->CreateBuffer(&cb_desc, nullptr, pConstantBuffer.GetAddressOf());
	}
};

export
class ColorModel :
	public BaseModel
{
public:
	ColorModel() = default;
	ColorModel(int x, int y, int width, int height, DirectX::XMFLOAT4 color)
		:
		x(x),
		y(y),
		width(width),
		height(height),
		color(color),
		angle(0.f)
	{}
	ColorModel(FRect rect, DirectX::XMFLOAT4 color)
		:
		x(static_cast<int>(rect.pos.x)),
		y(static_cast<int>(rect.pos.y)),
		width(static_cast<int>(rect.width)),
		height(static_cast<int>(rect.height)),
		color(color),
		angle(0.f)
	{}
	~ColorModel() override = default;

	DirectX::XMFLOAT2   GetOffset() const
	{
		return DirectX::XMFLOAT2(2 * static_cast<float>(x) / WND.GetWidth(), 2 * static_cast<float>(y) / WND.GetHeight());
	}
	DirectX::XMMATRIX   GetRotationMatrix() const
	{
		return DirectX::XMMATRIX(
			cosf(angle), -sinf(angle), 0.f, 0.f,
			sinf(angle), cosf(angle), 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		);
	}
	DirectX::XMMATRIX   GetScaleMatrix() const
	{
		return DirectX::XMMATRIX(
			static_cast<float>(width) / WND.GetWidth(), 0.f, 0.f, 0.f,
			0.f, static_cast<float>(height) / WND.GetHeight(), 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		);
	}
	DirectX::XMFLOAT4   GetColor() const
	{
		return color;
	}

	void                SetColor(const DirectX::XMFLOAT4& newColor)
	{
		color = newColor;
	}
	void                SetPosition(const FVec2D& newPos)
	{
		x = static_cast<int>(newPos.x);
		y = static_cast<int>(newPos.y);
	}
	void                SetRectangle(const FRect& newRect)
	{
		x = static_cast<int>(newRect.pos.x);
		y = static_cast<int>(newRect.pos.y);
		width = static_cast<int>(newRect.width);
		height = static_cast<int>(newRect.height);
	}

protected:
	virtual void        SettingShader() override
	{
		pShader = std::make_shared<ColorShader>();
		pShader->Initialize();
	}
	virtual void        SettingRenderingConstants() override
	{
		nVertices = 6;
		vertexBufferByteWidth = sizeof(ColorShader::VertexInput) * nVertices;
	}
	virtual void        SettingBufferConstants() override
	{
		stride = sizeof(ColorShader::VertexInput);
		offset = 0;
	}
	virtual void        SettingVertices() override
	{
		colorVertices[0] = { DirectX::XMFLOAT3{-1.f, -1.f, 0.f} };
		colorVertices[1] = { DirectX::XMFLOAT3{-1.f, 1.f, 0.f} };
		colorVertices[2] = { DirectX::XMFLOAT3{1.f, 1.f, 0.f} };
		colorVertices[3] = { DirectX::XMFLOAT3{-1.f, -1.f, 0.f} };
		colorVertices[4] = { DirectX::XMFLOAT3{1.f, 1.f, 0.f} };
		colorVertices[5] = { DirectX::XMFLOAT3{1.f, -1.f, 0.f} };

		pVertices = colorVertices;
		pInvertedVertices = colorVertices;
	}

private:
	int                         x;
	int                         y;
	int                         width;
	int                         height;
	DirectX::XMFLOAT4           color;
	float                       angle;

	ColorShader::VertexInput    colorVertices[6] = {};
};


export
class TextureShader :
	public BaseShader
{
public:
	export struct VertexInput
	{
		DirectX::XMFLOAT3   position;
		DirectX::XMFLOAT2   uv;
	};

	export struct VariableBuffer
	{
		DirectX::XMFLOAT2   offset;
		DirectX::XMMATRIX   rotation;
		DirectX::XMMATRIX   scale;
		float               depth;
	};

public:
	TextureShader() = default;

	virtual void        Initialize() override
	{
		HRESULT hr = S_OK;
		if (FAILED(hr = InputLayoutCreation()))
			throw GFX_EXCEPTION("An error has been caught during Shader Object Input Layout Creation.", hr);
		if (FAILED(hr = VertexShaderCreation()))
			throw GFX_EXCEPTION("An error has been caught during Shader Object Vertex Shader Creation.", hr);
		if (FAILED(hr = PixelShaderCreation()))
			throw GFX_EXCEPTION("An error has been caught during Shader Object Pixel Shader Creation.", hr);
		if (FAILED(hr = SampleStateCreation()))
			throw GFX_EXCEPTION("An error has been cauche during Shader Object Sample State Creation", hr);
		if (FAILED(hr = ConstantBufferCreation()))
			throw GFX_EXCEPTION("An error has been caught during Shader Object Constant Buffer Creation.", hr);

	}

	virtual void        RenderModel(BaseModel& model) override
	{
		HRESULT hr = S_OK;
		// Rendering Model
		GFX.GetDeviceContext()->IASetInputLayout(pInputLayout.Get());
		GFX.GetDeviceContext()->VSSetShader(pVertexShader.Get(), nullptr, 0);

		// We pass all the data needed for the Shader's Constant Buffer
		D3D11_MAPPED_SUBRESOURCE mapped_subresource;
		if (FAILED(hr = GFX.GetDeviceContext()->Map(pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource)))
			throw GFX_EXCEPTION("An exception has been caught during Shader Object Subresource Mapping.", hr);

		VariableBuffer* data = static_cast<VariableBuffer*>(mapped_subresource.pData);
		data->offset = dynamic_cast<TextureModel*>(&model)->GetOffset();
		data->rotation = dynamic_cast<TextureModel*>(&model)->GetRotationMatrix();
		data->scale = dynamic_cast<TextureModel*>(&model)->GetScaleMatrix();
		data->depth = model.GetDepth();

		GFX.GetDeviceContext()->Unmap(pConstantBuffer.Get(), 0);
		GFX.GetDeviceContext()->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());

		// Finishing rendering Model

		GFX.GetDeviceContext()->PSSetShaderResources(0, 1, dynamic_cast<TextureModel*>(&model)->GetTexture().GetShaderResourceView().GetAddressOf());
		GFX.GetDeviceContext()->PSSetShader(pPixelShader.Get(), nullptr, 0);
		GFX.GetDeviceContext()->PSSetSamplers(0, 1, pSampleState.GetAddressOf());
		GFX.GetDeviceContext()->Draw(model.GetVertices(), 0);

	}

protected:
	virtual HRESULT     InputLayoutCreation() override
	{
		constexpr D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		return GFX.GetDevice()->CreateInputLayout(ied, 2, Shaders::TextureVertexShaderBytecode, sizeof(Shaders::TextureVertexShaderBytecode), pInputLayout.GetAddressOf());
	}
	virtual HRESULT     VertexShaderCreation() override
	{
		return GFX.GetDevice()->CreateVertexShader(Shaders::TextureVertexShaderBytecode, sizeof(Shaders::TextureVertexShaderBytecode), nullptr, pVertexShader.GetAddressOf());
	}
	virtual HRESULT     PixelShaderCreation() override
	{
		return GFX.GetDevice()->CreatePixelShader(Shaders::TexturePixelShaderBytecode, sizeof(Shaders::TexturePixelShaderBytecode), nullptr, pPixelShader.GetAddressOf());
	}
	virtual HRESULT     SampleStateCreation()
	{
		D3D11_SAMPLER_DESC s_desc;
		ZeroMemory(&s_desc, sizeof(D3D11_SAMPLER_DESC));
		s_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		s_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		s_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		s_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		s_desc.MipLODBias = 0.f;
		s_desc.MaxAnisotropy = 1;
		s_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		s_desc.BorderColor[0] = 0;
		s_desc.BorderColor[1] = 0;
		s_desc.BorderColor[2] = 0;
		s_desc.BorderColor[3] = 0;
		s_desc.MinLOD = 0;
		s_desc.MaxLOD = D3D11_FLOAT32_MAX;

		return GFX.GetDevice()->CreateSamplerState(&s_desc, pSampleState.GetAddressOf());
	}
	virtual HRESULT     ConstantBufferCreation() override
	{
		D3D11_BUFFER_DESC cb_desc;
		ZeroMemory(&cb_desc, sizeof(D3D11_BUFFER_DESC));
		cb_desc.Usage = D3D11_USAGE_DYNAMIC;
		cb_desc.ByteWidth = sizeof(VariableBuffer);
		cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb_desc.MiscFlags = 0;
		cb_desc.StructureByteStride = 0;

		return GFX.GetDevice()->CreateBuffer(&cb_desc, nullptr, pConstantBuffer.GetAddressOf());
	}

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState>  pSampleState;
};


export
class TextureModel :
	public BaseModel
{
public:
	TextureModel() = default;
	~TextureModel() override = default;
	TextureModel(const std::string& texturePath)
		:
		texture(texturePath),
		width(texture.GetWidth()),
		height(texture.GetHeight())
	{}
	TextureModel(const Texture& texture)
		:
		texture(texture),
		width(texture.GetWidth()),
		height(texture.GetHeight())
	{}

	DirectX::XMFLOAT2   GetOffset() const
	{
		return DirectX::XMFLOAT2(2 * static_cast<float>(x) / WND.GetWidth(), 2 * static_cast<float>(y) / WND.GetHeight());
	}
	DirectX::XMMATRIX   GetRotationMatrix() const
	{
		return DirectX::XMMATRIX(
			cosf(angle), -sinf(angle), 0.f, 0.f,
			sinf(angle), cosf(angle), 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		);
	}
	DirectX::XMMATRIX   GetScaleMatrix() const
	{
		return DirectX::XMMATRIX(
			static_cast<float>(width) / WND.GetWidth(), 0.f, 0.f, 0.f,
			0.f, static_cast<float>(height) / WND.GetHeight(), 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		);
	}

	void                SetPosition(const FVec2D& newPos)
	{
		x = static_cast<int>(newPos.x);
		y = static_cast<int>(newPos.y);
	}
	void                SetRectangle(const FRect& newRectangle)
	{
		x = static_cast<int>(newRectangle.pos.x);
		y = static_cast<int>(newRectangle.pos.y);
		width = static_cast<int>(newRectangle.width);
		height = static_cast<int>(newRectangle.height);
	}
	void                SetTexture(const Texture& newTexture)
	{
		texture = newTexture;
		width = texture.GetWidth();
		height = texture.GetHeight();
	}
	Texture             GetTexture() const
	{
		return texture;
	}

protected:
	virtual void        SettingShader() override
	{
		pShader = std::make_shared<TextureShader>();
		pShader->Initialize();
	}
	virtual void        SettingRenderingConstants() override
	{
		nVertices = 6;
		vertexBufferByteWidth = sizeof(TextureShader::VertexInput) * nVertices;
	}
	virtual void        SettingBufferConstants() override
	{
		stride = sizeof(TextureShader::VertexInput);
		offset = 0;
	}
	virtual void        SettingVertices() override
	{
		textureVertices[0] = { DirectX::XMFLOAT3(-1.f, -1.f, 0.f), DirectX::XMFLOAT2(0.f, 1.f) };
		textureVertices[1] = { DirectX::XMFLOAT3(-1.f, 1.f, 0.f), DirectX::XMFLOAT2(0.f, 0.f) };
		textureVertices[2] = { DirectX::XMFLOAT3(1.f, -1.f, 0.f), DirectX::XMFLOAT2(1.f, 1.f) };
		textureVertices[3] = { DirectX::XMFLOAT3(-1.f, 1.f, 0.f), DirectX::XMFLOAT2(0.f, 0.f) };
		textureVertices[4] = { DirectX::XMFLOAT3(1.f, 1.f, 0.f), DirectX::XMFLOAT2(1.f, 0.f) };
		textureVertices[5] = { DirectX::XMFLOAT3(1.f, -1.f, 0.f), DirectX::XMFLOAT2(1.f, 1.f) };

		invertedTextureVertices[0] = { DirectX::XMFLOAT3(-1.f, -1.f, 0.f), DirectX::XMFLOAT2(1.f, 1.f) };
		invertedTextureVertices[1] = { DirectX::XMFLOAT3(-1.f, 1.f, 0.f), DirectX::XMFLOAT2(1.f, 0.f) };
		invertedTextureVertices[2] = { DirectX::XMFLOAT3(1.f, -1.f, 0.f), DirectX::XMFLOAT2(0.f, 1.f) };
		invertedTextureVertices[3] = { DirectX::XMFLOAT3(-1.f, 1.f, 0.f), DirectX::XMFLOAT2(1.f, 0.f) };
		invertedTextureVertices[4] = { DirectX::XMFLOAT3(1.f, 1.f, 0.f), DirectX::XMFLOAT2(0.f, 0.f) };
		invertedTextureVertices[5] = { DirectX::XMFLOAT3(1.f, -1.f, 0.f), DirectX::XMFLOAT2(0.f, 1.f) };

		pVertices = textureVertices;
		pInvertedVertices = invertedTextureVertices;
	}

private:
	Texture                     texture;
	int                         x = 0;
	int                         y = 0;
	int                         height = 0;
	int                         width = 0;
	float                       angle = 0.f;

	TextureShader::VertexInput  textureVertices[6] = {};
	TextureShader::VertexInput  invertedTextureVertices[6] = {};
};



export
class TextRenderer
{
public:
	TextRenderer()
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
	TextRenderer(const TextRenderer&) = delete;
	TextRenderer& operator=(const TextRenderer&) = delete;
	~TextRenderer() = default;

	static TextRenderer& GetInstance()
	{
		if (!pInstance)
			pInstance = std::make_unique<TextRenderer>();
		return *pInstance;
	}

	void					Render(const std::wstring& text, const std::wstring& fontFamily, float fontSize,
		FRect position, D2D1_COLOR_F fontColor = D2D1::ColorF(D2D1::ColorF::White))
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

public:
	static std::unique_ptr<TextRenderer>													pInstance;

	Microsoft::WRL::ComPtr<IDWriteFactory>													pDWriteFactory;
	Microsoft::WRL::ComPtr<ID2D1Factory>													pD2DFactory;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget>												pRenderTarget;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>											pTextBrush;

	std::map<std::pair<std::wstring, float>, Microsoft::WRL::ComPtr<IDWriteTextFormat>>		fonts;
};
// STATIC VARIABLES INITILIZATION
std::unique_ptr<TextRenderer> TextRenderer::pInstance = nullptr;

export TextRenderer& TEXT_ENGINE = TextRenderer::GetInstance();
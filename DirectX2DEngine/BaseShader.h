#pragma once
#include "DirectX.h"

class BaseShader
{
public:
	BaseShader() = default;
	virtual ~BaseShader() = default;

	virtual void Initialize();
	virtual void RenderModel(class BaseModel& model) = 0;

	ID3D11InputLayout* GetInputLayout() const;
	ID3D11VertexShader* GetVertexShader() const;
	ID3D11PixelShader* GetPixelShader() const;

protected:
	virtual HRESULT InputLayoutCreation() = 0;
	virtual HRESULT VertexShaderCreation() = 0;
	virtual HRESULT PixelShaderCreation() = 0;
	virtual HRESULT ConstantBufferCreation() = 0;

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};


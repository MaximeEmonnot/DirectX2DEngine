#include "BaseShader.h"

#include "Graphics.h"

BaseShader::~BaseShader()
{
	//pVertexShader->Release();
	//pInputLayout->Release();
	//pPixelShader->Release();
	//pConstantBuffer->Release();
}

void BaseShader::Initialize()
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

ID3D11InputLayout* BaseShader::GetInputLayout() const
{
	return pInputLayout.Get();
}

ID3D11VertexShader* BaseShader::GetVertexShader() const
{
	return pVertexShader.Get();
}

ID3D11PixelShader* BaseShader::GetPixelShader() const
{
	return pPixelShader.Get();
}

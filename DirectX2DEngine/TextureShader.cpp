#include "TextureShader.h"

#include "Graphics.h"
#include "BaseModel.h"
#include "TextureModel.h"

namespace Shaders
{
#include "TextureVertexShader.shh"
#include "TexturePixelShader.shh"
}

void TextureShader::Initialize()
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

void TextureShader::RenderModel(BaseModel& model)
{
	HRESULT hr = S_OK;
	// Rendering Model
	GFX.GetDeviceContext()->IASetInputLayout(pInputLayout.Get());
	GFX.GetDeviceContext()->VSSetShader(pVertexShader.Get(), nullptr, 0);

	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	if (FAILED(hr = GFX.GetDeviceContext()->Map(pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource)))
		throw GFX_EXCEPTION("An exception has been caught during Shader Object Subresource Mapping.", hr);

	VariableBuffer* data = static_cast<VariableBuffer*>(mapped_subresource.pData);
	data->translation = dynamic_cast<TextureModel*>(&model)->GetTranslationMatrix();
	data->rotation = dynamic_cast<TextureModel*>(&model)->GetRotationMatrix();
	data->scale = dynamic_cast<TextureModel*>(&model)->GetScaleMatrix();

	GFX.GetDeviceContext()->Unmap(pConstantBuffer.Get(), 0);
	GFX.GetDeviceContext()->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());

	GFX.GetDeviceContext()->PSSetShaderResources(0, 1, dynamic_cast<TextureModel*>(&model)->GetTexture().GetShaderResourceView().GetAddressOf());
	GFX.GetDeviceContext()->PSSetShader(pPixelShader.Get(), nullptr, 0);
	GFX.GetDeviceContext()->PSSetSamplers(0, 1, pSampleState.GetAddressOf());
	GFX.GetDeviceContext()->Draw(model.GetVertices(), 0);
}

HRESULT TextureShader::InputLayoutCreation()
{
	constexpr D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	return GFX.GetDevice()->CreateInputLayout(ied, 2, Shaders::TextureVertexShaderBytecode, sizeof(Shaders::TextureVertexShaderBytecode), pInputLayout.GetAddressOf());
}

HRESULT TextureShader::VertexShaderCreation()
{
	return GFX.GetDevice()->CreateVertexShader(Shaders::TextureVertexShaderBytecode, sizeof(Shaders::TextureVertexShaderBytecode), nullptr, pVertexShader.GetAddressOf());
}

HRESULT TextureShader::PixelShaderCreation()
{
	return GFX.GetDevice()->CreatePixelShader(Shaders::TexturePixelShaderBytecode, sizeof(Shaders::TexturePixelShaderBytecode), nullptr, pPixelShader.GetAddressOf());
}

HRESULT TextureShader::SampleStateCreation()
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

HRESULT TextureShader::ConstantBufferCreation()
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

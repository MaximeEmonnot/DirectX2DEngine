#include "ColorShader.h"

#include "BaseModel.h"
#include "ColorModel.h"
#include "Graphics.h"

namespace Shaders
{
#include "ColorVertexShader.shh"
#include "ColorPixelShader.shh"
}


void ColorShader::RenderModel(BaseModel& model)
{
	HRESULT hr = S_OK;
	// Rendering Model
	GFX.GetDeviceContext()->IASetInputLayout(pInputLayout.Get());
	GFX.GetDeviceContext()->VSSetShader(pVertexShader.Get(), nullptr, 0);

	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	if (FAILED(hr = GFX.GetDeviceContext()->Map(pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource)))
		throw GFX_EXCEPTION("An exception has been caught during Shader Object Subresource Mapping.", hr);

	VariableBuffer* data = static_cast<VariableBuffer*>(mapped_subresource.pData);
	data->offset = dynamic_cast<ColorModel*>(&model)->GetOffset();
	data->rotation = dynamic_cast<ColorModel*>(&model)->GetRotationMatrix();
	data->scale = dynamic_cast<ColorModel*>(&model)->GetScaleMatrix();
	data->color = dynamic_cast<ColorModel*>(&model)->GetColor();
		
	GFX.GetDeviceContext()->Unmap(pConstantBuffer.Get(), 0);
	GFX.GetDeviceContext()->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());

	GFX.GetDeviceContext()->PSSetShader(pPixelShader.Get(), nullptr, 0);
	GFX.GetDeviceContext()->Draw(model.GetVertices(), 0);
}

HRESULT ColorShader::InputLayoutCreation()
{
	constexpr D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	return GFX.GetDevice()->CreateInputLayout(ied, 1, Shaders::ColorVertexShaderBytecode, sizeof(Shaders::ColorVertexShaderBytecode), pInputLayout.GetAddressOf());
}

HRESULT ColorShader::VertexShaderCreation()
{
	return GFX.GetDevice()->CreateVertexShader(Shaders::ColorVertexShaderBytecode, sizeof(Shaders::ColorVertexShaderBytecode), nullptr, pVertexShader.GetAddressOf());
}

HRESULT ColorShader::PixelShaderCreation()
{
	return GFX.GetDevice()->CreatePixelShader(Shaders::ColorPixelShaderBytecode, sizeof(Shaders::ColorPixelShaderBytecode), nullptr, pPixelShader.GetAddressOf());
}

HRESULT ColorShader::ConstantBufferCreation()
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

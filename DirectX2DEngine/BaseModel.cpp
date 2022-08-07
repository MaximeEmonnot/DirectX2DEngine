#include "BaseModel.h"

#include "Graphics.h"

void BaseModel::Initialize()
{
	SettingShader();
	SettingBufferConstants();
	SettingRenderingConstants();
	SettingVertices();
	SettingIndices();

	HRESULT hr = S_OK;

	if (FAILED(hr = SettingVertexBuffer()))
		throw GFX_EXCEPTION("An exception has been caught during Model Vertex Buffer Initialization.", hr);

	if (FAILED(hr = SettingIndexBuffer()))
		throw GFX_EXCEPTION("An exception has been caught during Model Index Buffer Initialization.", hr);
}

void BaseModel::Render()
{
	// Render Model
	GFX.GetDeviceContext()->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);
	GFX.GetDeviceContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	GFX.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Render Shader
	pShader->RenderModel(*this);
}

int BaseModel::GetVertices() const
{
	return nVertices;
}

void BaseModel::SettingIndices()
{
	indices = new unsigned long[nVertices];
	for (int i = 0; i < nVertices; i++)
		indices[i] = i;
}

HRESULT BaseModel::SettingVertexBuffer()
{
	D3D11_BUFFER_DESC vb_desc;
	ZeroMemory(&vb_desc, sizeof(D3D11_BUFFER_DESC));
	vb_desc.Usage = D3D11_USAGE_DEFAULT;
	vb_desc.ByteWidth = vertexBufferByteWidth;
	vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vb_data;
	ZeroMemory(&vb_data, sizeof(D3D11_SUBRESOURCE_DATA));
	vb_data.pSysMem = vertices;

	return GFX.GetDevice()->CreateBuffer(&vb_desc, &vb_data, &pVertexBuffer);
}

HRESULT BaseModel::SettingIndexBuffer()
{
	D3D11_BUFFER_DESC ib_desc;
	ZeroMemory(&ib_desc, sizeof(D3D11_BUFFER_DESC));
	ib_desc.Usage = D3D11_USAGE_DEFAULT;
	ib_desc.ByteWidth = sizeof(unsigned long) * nVertices;
	ib_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA ib_data;
	ZeroMemory(&ib_data, sizeof(D3D11_SUBRESOURCE_DATA));
	ib_data.pSysMem = indices;

	return GFX.GetDevice()->CreateBuffer(&ib_desc, &ib_data, &pIndexBuffer);
}

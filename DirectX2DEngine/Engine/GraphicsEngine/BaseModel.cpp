#include "GraphicsEngine/BaseModel.h"

#include "GraphicsEngine/Graphics.h"

BaseModel::~BaseModel()
{
	//pVertexBuffer->Release();
	//pInvertedVertexBuffer->Release();
	//pIndexBuffer->Release();
}

void BaseModel::Initialize()
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

void BaseModel::Render()
{
	// Render Model
	if (bIsInverted) GFX.GetDeviceContext()->IASetVertexBuffers(0, 1, pInvertedVertexBuffer.GetAddressOf(), &stride, &offset);
	else GFX.GetDeviceContext()->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);
	GFX.GetDeviceContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	GFX.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Render Shader
	pShader->RenderModel(*this);
}

int BaseModel::GetVertices() const
{
	return nVertices;
}

void BaseModel::SetDepth(float new_depth)
{
	depth = new_depth;
}

float BaseModel::GetDepth() const
{
	return (512.f - depth) / 512.f;
}

void BaseModel::SetVisibility(bool bValue)
{
	bIsVisible = bValue;
}

bool BaseModel::IsVisible() const
{
	return bIsVisible;
}

void BaseModel::SetInverted(bool bValue)
{
	bIsInverted = bValue;
}

bool BaseModel::IsInverted() const
{
	return bIsInverted;
}

void BaseModel::SettingIndices()
{
	indices = new unsigned long[nVertices];
	for (int i = 0; i < nVertices; i++)
		indices[i] = i;
}

HRESULT BaseModel::SettingVertexBuffers()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC vb_desc;
	ZeroMemory(&vb_desc, sizeof(D3D11_BUFFER_DESC));
	vb_desc.Usage = D3D11_USAGE_DEFAULT;
	vb_desc.ByteWidth = vertexBufferByteWidth;
	vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vb_data;
	ZeroMemory(&vb_data, sizeof(D3D11_SUBRESOURCE_DATA));
	vb_data.pSysMem = vertices;

	hr = GFX.GetDevice()->CreateBuffer(&vb_desc, &vb_data, &pVertexBuffer);

	D3D11_BUFFER_DESC ivb_desc;
	ZeroMemory(&ivb_desc, sizeof(D3D11_BUFFER_DESC));
	ivb_desc.Usage = D3D11_USAGE_DEFAULT;
	ivb_desc.ByteWidth = vertexBufferByteWidth;
	ivb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA ivb_data;
	ZeroMemory(&ivb_data, sizeof(D3D11_SUBRESOURCE_DATA));
	ivb_data.pSysMem = inverted_vertices;

	hr = GFX.GetDevice()->CreateBuffer(&ivb_desc, &ivb_data, &pInvertedVertexBuffer);

	return hr;
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

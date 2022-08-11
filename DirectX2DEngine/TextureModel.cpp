#include "TextureModel.h"

#include "Window.h"

TextureModel::TextureModel(const std::string& texture_path)
	:
	texture(texture_path)
{
}

TextureModel::TextureModel(Texture& texture)
	:
	texture(texture)
{
}

DirectX::XMFLOAT2 TextureModel::GetOffset() const
{
	return DirectX::XMFLOAT2(static_cast<float>(x) / WND.GetWidth(), static_cast<float>(y) / WND.GetHeight());
}

DirectX::XMMATRIX TextureModel::GetRotationMatrix() const
{
    return DirectX::XMMATRIX(
		cosf(angle), -sinf(angle), 0.f, 0.f,
		sinf(angle), cosf(angle), 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

DirectX::XMMATRIX TextureModel::GetScaleMatrix() const
{
    return DirectX::XMMATRIX(
		static_cast<float>(texture.GetWidth()) / WND.GetWidth(), 0.f, 0.f, 0.f,
		0.f, static_cast<float>(texture.GetHeight()) / WND.GetHeight(), 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

void TextureModel::SetTexture(const Texture& tex)
{
	texture = tex;
}

Texture TextureModel::GetTexture() const
{
	return texture;
}

void TextureModel::SettingShader()
{
	pShader = std::make_shared<TextureShader>();
	pShader->Initialize();
}

void TextureModel::SettingRenderingConstants()
{
	nVertices = 6;
	vertexBufferByteWidth = sizeof(TextureShader::VertexInput) * nVertices;
}

void TextureModel::SettingBufferConstants()
{
	stride = sizeof(TextureShader::VertexInput);
	offset = 0;
}

void TextureModel::SettingVertices()
{
	texture_vertices[0] = { DirectX::XMFLOAT3(-1.f, -1.f, 0.f), DirectX::XMFLOAT2(0.f, 1.f) };
	texture_vertices[1] = { DirectX::XMFLOAT3(-1.f, 1.f, 0.f), DirectX::XMFLOAT2(0.f, 0.f) };
	texture_vertices[2] = { DirectX::XMFLOAT3(1.f, -1.f, 0.f), DirectX::XMFLOAT2(1.f, 1.f) };
	texture_vertices[3] = { DirectX::XMFLOAT3(-1.f, 1.f, 0.f), DirectX::XMFLOAT2(0.f, 0.f) };
	texture_vertices[4] = { DirectX::XMFLOAT3(1.f, 1.f, 0.f), DirectX::XMFLOAT2(1.f, 0.f) };
	texture_vertices[5] = { DirectX::XMFLOAT3(1.f, -1.f, 0.f), DirectX::XMFLOAT2(1.f, 1.f) };

	vertices = texture_vertices;
}

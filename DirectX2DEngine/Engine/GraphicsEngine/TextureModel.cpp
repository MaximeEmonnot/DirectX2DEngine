#include "GraphicsEngine/TextureModel.h"

#include "CoreEngine/Window.h"

TextureModel::TextureModel(const std::string& texture_path)
	:
	texture(texture_path),
	width(texture.GetWidth()),
	height(texture.GetHeight())
{
}

TextureModel::TextureModel(const Texture& texture)
	:
	texture(texture),
	width(texture.GetWidth()),
	height(texture.GetHeight())
{
}

DirectX::XMFLOAT2 TextureModel::GetOffset() const
{
	return DirectX::XMFLOAT2(2 * static_cast<float>(x) / WND.GetWidth(), 2 * static_cast<float>(y) / WND.GetHeight());
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
		static_cast<float>(width) / WND.GetWidth(), 0.f, 0.f, 0.f,
		0.f, static_cast<float>(height) / WND.GetHeight(), 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

void TextureModel::SetPosition(const FVec2D& newPos)
{
	x = static_cast<int>(newPos.x);
	y = static_cast<int>(newPos.y);
}

void TextureModel::SetRectangle(const FRect& newRectangle)
{
	x = static_cast<int>(newRectangle.pos.x);
	y = static_cast<int>(newRectangle.pos.y);
	width = static_cast<int>(newRectangle.width);
	height = static_cast<int>(newRectangle.height);
}

void TextureModel::SetTexture(const Texture& newTexture)
{
	texture = newTexture;
	width = texture.GetWidth();
	height = texture.GetHeight();
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

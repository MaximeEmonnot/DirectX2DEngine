#include "GraphicsEngine/ColorModel.h"

#include "CoreEngine/Window.h"
#include "GraphicsEngine/ColorShader.h"

ColorModel::ColorModel(int x, int y, int width, int height, DirectX::XMFLOAT4 color)
	:
	x(x),
	y(y),
	width(width),
	height(height),
	color(color),
	angle(0.f)
{
}

ColorModel::ColorModel(FRect rect, DirectX::XMFLOAT4 color)
	:
	x(static_cast<int>(rect.pos.x)),
	y(static_cast<int>(rect.pos.y)),
	width(static_cast<int>(rect.width)),
	height(static_cast<int>(rect.height)),
	color(color),
	angle(0.f)
{
}

DirectX::XMFLOAT2 ColorModel::GetOffset() const
{
	return DirectX::XMFLOAT2(2 * static_cast<float>(x) / WND.GetWidth(), 2 * static_cast<float>(y) / WND.GetHeight());
}

DirectX::XMMATRIX ColorModel::GetRotationMatrix() const
{
	return DirectX::XMMATRIX(
		cosf(angle), -sinf(angle), 0.f, 0.f,
		sinf(angle), cosf(angle), 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

DirectX::XMMATRIX ColorModel::GetScaleMatrix() const
{
	return DirectX::XMMATRIX(
		static_cast<float>(width) / WND.GetWidth(), 0.f, 0.f, 0.f,
		0.f, static_cast<float>(height) / WND.GetHeight(), 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

DirectX::XMFLOAT4 ColorModel::GetColor() const
{
	return color;
}

void ColorModel::SetColor(const DirectX::XMFLOAT4& newColor)
{
	color = newColor;
}

void ColorModel::SetPosition(const FVec2D& newPos)
{
	x = static_cast<int>(newPos.x);
	y = static_cast<int>(newPos.y);
}

void ColorModel::SetRectangle(const FRect& newRect)
{
	x = static_cast<int>(newRect.pos.x);
	y = static_cast<int>(newRect.pos.y);
	width = static_cast<int>(newRect.width);
	height = static_cast<int>(newRect.height);
}

void ColorModel::SettingShader()
{
	pShader = std::make_shared<ColorShader>();
	pShader->Initialize();
}

void ColorModel::SettingRenderingConstants()
{
	nVertices = 6;
	vertexBufferByteWidth = sizeof(ColorShader::VertexInput) * nVertices;
}

void ColorModel::SettingBufferConstants()
{
	stride = sizeof(ColorShader::VertexInput);
	offset = 0;
}

void ColorModel::SettingVertices()
{
	colorVertices[0] = { DirectX::XMFLOAT3{-1.f, -1.f, 0.f}};
	colorVertices[1] = { DirectX::XMFLOAT3{-1.f, 1.f, 0.f}};
	colorVertices[2] = { DirectX::XMFLOAT3{1.f, 1.f, 0.f}};
	colorVertices[3] = { DirectX::XMFLOAT3{-1.f, -1.f, 0.f} };
	colorVertices[4] = { DirectX::XMFLOAT3{1.f, 1.f, 0.f}};
	colorVertices[5] = { DirectX::XMFLOAT3{1.f, -1.f, 0.f}};

	pVertices = colorVertices;
	pInvertedVertices = colorVertices;
}

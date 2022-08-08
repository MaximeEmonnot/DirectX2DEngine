#include "ColorModel.h"
#include "ColorShader.h"
#include "Window.h"

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

void ColorModel::Update()
{
	if (MOUSE.Read() == Mouse::EventType::WheelUp) 
		angle += 0.01f;
	if (MOUSE.Read() == Mouse::EventType::WheelDown) 
		angle -= 0.01f;
}

DirectX::XMMATRIX ColorModel::GetTranslationMatrix() const
{
	return DirectX::XMMATRIX(
		1.f, 0.f, 0.f, static_cast<float>(x) / WND.GetWidth(),
		0.f, 1.f, 0.f, static_cast<float>(y) / WND.GetHeight(),
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
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
	color_vertices[0] = { DirectX::XMFLOAT3{-1.f, -1.f, 0.5f}};
	color_vertices[1] = { DirectX::XMFLOAT3{-1.f, 1.f, 0.5f}};
	color_vertices[2] = { DirectX::XMFLOAT3{1.f, 1.f, 0.5f}};
	color_vertices[3] = { DirectX::XMFLOAT3{-1.f, -1.f, 0.5f} };
	color_vertices[4] = { DirectX::XMFLOAT3{1.f, 1.f, 0.5f}};
	color_vertices[5] = { DirectX::XMFLOAT3{1.f, -1.f, 0.5f}};

	vertices = color_vertices;
}

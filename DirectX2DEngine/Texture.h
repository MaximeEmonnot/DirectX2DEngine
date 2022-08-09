#pragma once
#include <string>

#include "DirectX.h"

class Texture
{
private:
	struct TGAHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};
public:
	Texture() = default;
	Texture(const std::string& filepath);
	Texture(const Texture& tex);

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() const;

	int GetWidth() const;
	int GetHeight() const;

private:
	unsigned char* LoadTGA(const std::string& filepath);

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;

	int width = 0;
	int height = 0;
};


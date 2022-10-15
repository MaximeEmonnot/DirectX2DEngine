#include "GraphicsEngine/Texture.h"

#include "GraphicsEngine/Graphics.h"

Texture::Texture(const std::string& filepath)
{
	HRESULT hr = S_OK;

	unsigned char* tga_data = LoadTGA(filepath);

	D3D11_TEXTURE2D_DESC tex_desc;
	ZeroMemory(&tex_desc, sizeof(D3D11_TEXTURE2D_DESC));

	tex_desc.Width = width;
	tex_desc.Height = height;
	tex_desc.MipLevels = 0;
	tex_desc.ArraySize = 1;
	tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	tex_desc.CPUAccessFlags = 0;
	tex_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	if (FAILED(hr = GFX.GetDevice()->CreateTexture2D(&tex_desc, nullptr, pTexture.GetAddressOf())))
		throw GFX_EXCEPTION("An exception has been caught during Texture Object Creation.", hr);

	const int row_pitch = (width * 4) * sizeof(unsigned char);
	GFX.GetDeviceContext()->UpdateSubresource(pTexture.Get(), 0, nullptr, tga_data, row_pitch, 0);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory(&srv_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	srv_desc.Format = tex_desc.Format;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MostDetailedMip = 0;
	srv_desc.Texture2D.MipLevels = -1;

	if (FAILED(hr = GFX.GetDevice()->CreateShaderResourceView(pTexture.Get(), &srv_desc, pTextureView.GetAddressOf())))
		throw GFX_EXCEPTION("An exception has been caught during Texture Object Shader Resource View Creation.", hr);

	GFX.GetDeviceContext()->GenerateMips(pTextureView.Get());

	delete[] tga_data;
	tga_data = nullptr;
}

Texture::Texture(const Texture& tex)
	:
	pTexture(tex.pTexture),
	pTextureView(tex.pTextureView),
	width(tex.width),
	height(tex.height)
{
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Texture::GetShaderResourceView() const
{
	return pTextureView;
}

int Texture::GetWidth() const
{
	return width;
}

int Texture::GetHeight() const
{
	return height;
}

unsigned char* Texture::LoadTGA(const std::string& filepath)
{
	FILE* pFile;
	TGAHeader tgaFileHeader = {};
	fopen_s(&pFile, filepath.c_str(), "rb");

	unsigned int count = static_cast<unsigned int>(fread(&tgaFileHeader, sizeof(TGAHeader), 1, pFile));

	width = static_cast<int>(tgaFileHeader.width);
	height = static_cast<int>(tgaFileHeader.height);

	if (tgaFileHeader.bpp != 32)
		throw GFX_EXCEPTION("An exception has occured during TGA file reading. Please use TGA 32 bits.", S_FALSE);

	const int imageSize = width * height * 4;

	unsigned char* tgaImage = new unsigned char[imageSize];

	fread(tgaImage, 1, imageSize, pFile);

	fclose(pFile);

	unsigned char* tga_data = new unsigned char[imageSize];

	int index = 0;
	int k = (width * height * 4) - (width * 4);

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			tga_data[index + 0] = tgaImage[k + 2];
			tga_data[index + 1] = tgaImage[k + 1];
			tga_data[index + 2] = tgaImage[k + 0];
			tga_data[index + 3] = tgaImage[k + 3];

			k += 4;
			index += 4;
		}
		k -= (width * 8);
	}

	delete[] tgaImage;
	tgaImage = nullptr;

	return tga_data;
}
